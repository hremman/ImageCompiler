#ifndef CIMAGESTORAGE_HPP
#define CIMAGESTORAGE_HPP

#include <QTemporaryFile>
#include <QImage>
#include <list>
#include <vector>
#include <map>

class CImageStorage
{
public:
    using iid_t = unsigned long long;
    static const  iid_t __M_ID_MAX = 0xFFFFFFFFFFFFFFFE;
    static const  iid_t ERROR = 0xFFFFFFFFFFFFFFFE;

    enum sizes_e : long long
    {
        BYTE = 1,
        B = 1,
        KB = 1024,
        MB = 1024 * KB,
        GB = 1024 * MB
    };


protected:
    long long m_limitation;
    class ImageHolder
    {
    public:
        std::list<QImage>::iterator m_img;
        qint64 m_in_file;
        QString m_filename;
        iid_t m_id;
        qint64 m_size;
    };

public:
    CImageStorage(long long limit = 8 * GB);

    iid_t put(const QString &, const QImage &);
    iid_t put(const QImage &);
    iid_t put(const QString &);

    QImage getCopy(iid_t);
    const QImage & get(iid_t);
    QImage & get(iid_t, QImage &);

    QImage getCopy(const QString &);
    const QImage & get(const QString &);
    QImage & get(const QString &, QImage &);

    bool isEmpty() const
        { return m_index_primary.size() == 0; }

    size_t count() const
        { return m_index_primary.size(); }

    size_t sizeInBytes() const
        { return m_file.size();}

    void clear();

    bool contains(iid_t) const;
    bool contains(const QString &) const;

    const QString & getName(iid_t) const;
    iid_t getId(const QString &) const;


    ~CImageStorage()
    {
        if (m_file.isOpen())
            m_file.close();
    }

protected:
    qint64 to_file(const QImage &);
    QImage & from_file(qint64, QImage &);

protected:
    QTemporaryFile m_file;

    std::map<iid_t, std::list<ImageHolder>::iterator>   m_index_primary;
    std::map<std::list<ImageHolder>::iterator, iid_t>   m_index_primary_reverse;
    std::map<QString, iid_t>                            m_index_reverse;
    std::map<std::list<QImage>::iterator, iid_t>        m_index_loaded_reverse;
    std::list<QImage> m_images;
    std::list<ImageHolder> m_holders;
    long long m_size_in_memory;
    long long m_size_in_file;
};

#endif // CIMAGESTORAGE_HPP
