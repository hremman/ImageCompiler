#ifndef SPINLOCK_HPP
#define SPINLOCK_HPP

#include <unistd.h>
#include <time.h>


class CSpinLock
{
public:
    CSpinLock()
        : m_lock(false)
    {}

    __attribute__((always_inline))
    void unlock()
        {__sync_lock_test_and_set(&m_lock, false);}

    __attribute__((always_inline))
    bool tryLock()
        { return __sync_lock_test_and_set(&m_lock, true); }

    __attribute__((always_inline))
    void lock()
    {
        unsigned nTry = 10;
        while (tryLock())
        {
            if (nTry != 0)
            {
                --nTry;
                nanosleep(&nanosecs, 0);
            }
            else
                usleep(1);
        }
    }

protected:
    bool m_lock;
    static constexpr struct timespec nanosecs = {0,100};
};

#endif // SPINLOCK_HPP
