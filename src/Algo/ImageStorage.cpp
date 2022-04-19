
#include "ImageStorage.hpp"
#include "Exceptions.hpp"


CImageStorage::CImageStorage(long long limit)
    : m_limitation(limit)
    , m_file()
    , m_index_primary()
    , m_index_primary_reverse()
    , m_index_reverse()
    , m_index_loaded_reverse()
    , m_images()
    , m_holders()
    , m_size_in_memory(0)
    , m_size_in_file(0)
{}

CImageStorage::iid_t CImageStorage::put(const QString &file, const QImage &img)
{
    ImageHolder holder;
    iid_t temp_id = 0;
    while ( m_index_primary.find(temp_id) != m_index_primary.end() )
        ++temp_id;
    if ( temp_id == ERROR)
        throw TooBigId("Свободные идентификаторы концились!");
    holder.m_id = temp_id;

    holder.m_filename = file;
    holder.m_size = img.sizeInBytes();

    if ( m_limitation > m_size_in_memory && m_limitation - m_size_in_memory > img.sizeInBytes() )
    {
        m_images.push_back(img);
        holder.m_img = (--m_images.end());
        holder.m_in_file = -1;
        m_size_in_memory += holder.m_size;
        m_index_loaded_reverse[holder.m_img] = holder.m_id;
    }
    else
    {
        holder.m_img = m_images.end();
        holder.m_in_file = to_file(img);
        m_size_in_file += holder.m_size;
    }

    m_holders.push_back(holder);
    m_index_primary[holder.m_id] = --m_holders.end();
    m_index_primary_reverse[--m_holders.end()] = holder.m_id;
    m_index_reverse[holder.m_filename] = holder.m_id;
    return temp_id;
}

CImageStorage::iid_t CImageStorage::put(const QImage & img)
{
    ImageHolder holder;
    iid_t temp_id = 0;
    while ( m_index_primary.find(temp_id) != m_index_primary.end() )
        ++temp_id;
    if ( temp_id == ERROR)
        throw TooBigId("Свободные идентификаторы концились!");
    holder.m_id = temp_id;
    holder.m_size = img.sizeInBytes();
    if ( m_limitation > m_size_in_memory && m_limitation - m_size_in_memory > img.sizeInBytes() )
    {
        m_images.push_back(img);
        holder.m_img = (--m_images.end());
        holder.m_in_file = -1;
        m_size_in_memory += holder.m_size;
        m_index_loaded_reverse[holder.m_img] = holder.m_id;
    }
    else
    {
        holder.m_img = m_images.end();
        holder.m_in_file = to_file(img);
        m_size_in_file += holder.m_size;
    }

    m_holders.push_back(holder);
    m_index_primary[holder.m_id] = --m_holders.end();
    m_index_primary_reverse[--m_holders.end()] = holder.m_id;

    return temp_id;
}

CImageStorage::iid_t CImageStorage::put(const QString & file)
{
    QImage img(file);
    if ( img.isNull() )
        throw LoadImageError("Не удалось загрузить указанный файл");

    return put(file, img);
}

QImage CImageStorage::getCopy(iid_t id)
{
    auto holder = m_index_primary.find(id);
    if ( holder == m_index_primary.end() )
        throw NoSuchImage("Изображение с идентификатором " + QString::number(id) + " нет в хранилище");

    if ( holder->second->m_img != m_images.end() )
    {
        QImage temp(std::move(*(holder->second->m_img)));
        m_images.erase(holder->second->m_img);
        m_images.emplace_back(std::move(temp));
        holder->second->m_img = --m_images.end();
        return *(holder->second->m_img);
    }

    while ( m_limitation - m_size_in_memory < holder->second->m_in_file && m_images.size() )
    {
        auto to_unload = m_images.begin();
        auto id_u = m_index_loaded_reverse[to_unload];
        auto holder_u = m_index_primary.at(id_u);
        holder_u->m_img = m_images.end();
        if ( holder_u->m_in_file == -1 )
            holder_u->m_in_file = to_file(*to_unload);

        m_size_in_memory -= to_unload->sizeInBytes();
        m_images.erase(to_unload);
    }

    if ( m_limitation - m_size_in_memory > holder->second->m_in_file )
    {
        m_images.emplace_back();
        from_file(holder->second->m_in_file, m_images.back());
        holder->second->m_img = --m_images.end();
        m_size_in_memory += m_images.back().sizeInBytes();
        m_index_loaded_reverse[holder->second->m_img] = holder->second->m_id;
        return m_images.back();
    }

    QImage img;
    from_file(holder->second->m_in_file, img);
    return img;
}

const QImage & CImageStorage::get(iid_t id)
{
    auto holder = m_index_primary.find(id);
    if ( holder == m_index_primary.end() )
        throw NoSuchImage("Изображение с идентификатором " + QString::number(id) + " нет в хранилище");

    if ( holder->second->m_img != m_images.end() )
    {
        QImage temp(std::move(*(holder->second->m_img)));
        m_images.erase(holder->second->m_img);
        m_images.emplace_back(std::move(temp));
        holder->second->m_img = --m_images.end();
        return *(holder->second->m_img);
    }

    while ( m_limitation - m_size_in_memory < holder->second->m_in_file && m_images.size() )
    {
        auto to_unload = m_images.begin();
        auto id_u = m_index_loaded_reverse[to_unload];
        auto holder_u = m_index_primary.at(id_u);
        holder_u->m_img = m_images.end();
        if ( holder_u->m_in_file == -1 )
            holder_u->m_in_file = to_file(*to_unload);

        m_size_in_memory -= to_unload->sizeInBytes();
        m_images.erase(to_unload);
    }

    m_images.emplace_back();
    from_file(holder->second->m_in_file, m_images.back());
    holder->second->m_img = --m_images.end();
    m_size_in_memory += m_images.back().sizeInBytes();
    m_index_loaded_reverse[holder->second->m_img] = holder->second->m_id;
    return m_images.back();
}

QImage & CImageStorage::get(iid_t id, QImage &img)
{
    auto holder = m_index_primary.find(id);
    if ( holder == m_index_primary.end() )
        throw NoSuchImage("Изображение с идентификатором " + QString::number(id) + " нет в хранилище");

    if ( holder->second->m_img != m_images.end() )
    {
        QImage temp(std::move(*(holder->second->m_img)));
        m_images.erase(holder->second->m_img);
        m_images.emplace_back(std::move(temp));
        holder->second->m_img = --m_images.end();
        img = *(holder->second->m_img);
        return img;
    }

    while ( m_limitation - m_size_in_memory < holder->second->m_in_file && m_images.size() )
    {
        auto to_unload = m_images.begin();
        auto id_u = m_index_loaded_reverse[to_unload];
        auto holder_u = m_index_primary.at(id_u);
        holder_u->m_img = m_images.end();
        if ( holder_u->m_in_file == -1 )
            holder_u->m_in_file = to_file(*to_unload);

        m_size_in_memory -= to_unload->sizeInBytes();
        m_images.erase(to_unload);
    }

    if ( m_limitation - m_size_in_memory > holder->second->m_in_file )
    {
        m_images.emplace_back();
        from_file(holder->second->m_in_file, m_images.back());
        holder->second->m_img = --m_images.end();
        m_size_in_memory += m_images.back().sizeInBytes();
        m_index_loaded_reverse[holder->second->m_img] = holder->second->m_id;
        img = m_images.back();
        return img;
    }

    from_file(holder->second->m_in_file, img);
    return img;
}

QImage CImageStorage::getCopy(const QString & file)
{
    auto record = m_index_reverse.find(file);
    if ( record == m_index_reverse.end() )
        throw NoSuchImage("Изображение с путём \"" + file + "\" нет в хранилище");
    QImage img;
    return get(record->second, img);
}
const QImage & CImageStorage::get(const QString & file)
{
    auto record = m_index_reverse.find(file);
    if ( record == m_index_reverse.end() )
        throw NoSuchImage("Изображение с путём \"" + file + "\" нет в хранилище");
    return get(record->second);
}
QImage & CImageStorage::get(const QString & file, QImage & img)
{
    auto record = m_index_reverse.find(file);
    if ( record == m_index_reverse.end() )
        throw NoSuchImage("Изображение с путём \"" + file + "\" нет в хранилище");
    return get(record->second, img);
}

bool CImageStorage::contains(iid_t id) const
    { return m_index_primary.find(id) != m_index_primary.end(); }


bool CImageStorage::contains(const QString & file) const
    { return m_index_reverse.find(file) != m_index_reverse.end(); }



qint64 CImageStorage::to_file(const QImage & img)
{
    if ( !m_file.isOpen() )
        m_file.open();
    QByteArray data(img.sizeInBytes(), Qt::Uninitialized);
    memcpy(data.data(), img.constBits(), img.sizeInBytes());
    m_file.seek(m_file.size());
    qint64 pos = m_file.pos();
    qint64 size = static_cast<qint64>(data.size());
    m_file.write(reinterpret_cast<const char *>(&size), sizeof (size));
    m_file.write(data.data(), size);
    m_size_in_file += img.sizeInBytes();
    return pos;
}

QImage & CImageStorage::from_file(qint64 pos, QImage & img)
{
    m_file.seek(pos);
    qint64 size = 0;
    m_file.read(reinterpret_cast<char *>(&size), sizeof (size));
    QByteArray data(size, Qt::Uninitialized);
    m_file.read(data.data(), size);
    img.fromData(data);
    return img;

}

const QString & CImageStorage::getName(iid_t id) const
{
    auto holder = m_index_primary.find(id);
    if ( holder == m_index_primary.end() )
        throw NoSuchImage("Изображение с идентификатором " + QString::number(id) + " нет в хранилище");
    return holder->second->m_filename;
}

CImageStorage::iid_t CImageStorage::getId(const QString & file) const
{
    auto record = m_index_reverse.find(file);
    if ( record == m_index_reverse.end() )
        throw NoSuchImage("Изображение с путём \"" + file + "\" нет в хранилище");
    return record->second;
}
