#include <QImageReader>
#include "PreviewBuilder.hpp"
#include "Algo/CompilerTask.hpp"
#include "Algo/ImageStorage.hpp"

QImage CPreviewBuilder::buildPreview(std::vector<QString> files, std::vector<Data::CColor *> colors)
{
    CImageStorage preview_storage(50 * CImageStorage::MB);
    TaskStorage task(preview_storage);

    QSize max(0,0);

    for (auto file = files.begin(); file != files.end(); ++file)
    {
        QImageReader img_info(*file);
        if ( img_info.size().height() > max.height() || img_info.size().width() > max.width() )
            max = img_info.size();
    }
    std::vector<CImageStorage::iid_t> images;
    images.resize(files.size());
    if ( max.height() > 600 || max.width() > 600 )
    {
        qreal factor = 600 / std::max(max.height(), max.width());
        for (size_t it = 0; it < files.size(); ++it)
        {
            QImage temp_img(files[it]);
            images[it] = preview_storage.put(temp_img.scaled(temp_img.size() * factor, Qt::KeepAspectRatio));
        }
    }
    else
    {
        for (size_t it = 0; it < files.size(); ++it)
        {
            QImage temp_img(files[it]);
            images[it] = preview_storage.put(temp_img);
        }
    }

    int threads = CCompiler::threadsAvalible();


    QFutureWatcher<bool> * watchdog = new QFutureWatcher<bool>[threads] ;
    std::vector<ImageCache> cache;
    cache.reserve(threads+1);
    cache.resize(threads);
    for(size_t it = 0; it < files.size(); ++it)
    {
        const QImage &img = preview_storage.get(images[it]);
        for (int i = 0 ; i < threads; i++)
        {
            //watchdog.emplace_back(nullptr);
            watchdog[i].setFuture(QtConcurrent::run([&]() -> bool { return CImageProcessing::getAverage(img, static_cast<size_t>(0), static_cast<size_t>(threads), cache[i].m_saturation, cache[i].m_value);}));
        }
        for (int i = 0 ; i < threads; i++)
            watchdog[i].waitForFinished();
        task.push(images[it], colors[it], CImageProcessing::CollapseCache(cache.data(), threads));
    }
    delete [] watchdog;




    QImage ret;
    CCompiler::BuildOne(&task, ret);
    return ret;
}
