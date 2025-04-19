#ifndef SNOWFLAKE_HPP
#define SNOWFLAKE_HPP

#include <chrono>
#include <mutex>
#include <stdexcept>
#include <cstdint>
#include <thread>

class SnowflakeGenerator
{
private:
    // Bit lengths for different parts of the ID
    static constexpr int TIMESTAMP_BITS = 41;
    static constexpr int WORKER_ID_BITS = 10;
    static constexpr int SEQUENCE_BITS = 12;

    // Masks and shifts
    static constexpr int64_t MAX_WORKER_ID = (1LL << WORKER_ID_BITS) - 1;
    static constexpr int64_t MAX_SEQUENCE = (1LL << SEQUENCE_BITS) - 1;

    static constexpr int WORKER_ID_SHIFT = SEQUENCE_BITS;
    static constexpr int TIMESTAMP_SHIFT = SEQUENCE_BITS + WORKER_ID_BITS;

    // Custom epoch (e.g., Jan 1, 2020 UTC) because I am arbitrary
    static constexpr int64_t EPOCH = 1577836800000; // ms since Unix epoch

    int64_t workerId;
    int64_t lastTimestamp;
    int64_t sequence;
    std::mutex mutex;

    /**
     * Get current timestamp in milliseconds
     */
    int64_t currentTimeMillis() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    }

    /**
     * Block until next millisecond
     */
    int64_t waitNextMillis(int64_t currentTimestamp) const
    {
        while (currentTimestamp <= lastTimestamp)
        {
            currentTimestamp = currentTimeMillis();
        }
        return currentTimestamp;
    }

public:
    /**
     * Constructor
     *
     * @param workerId Worker ID (0-1023)
     */
    explicit SnowflakeGenerator(int64_t workerId) : workerId(workerId),
                                                    lastTimestamp(-1LL),
                                                    sequence(0LL)
    {

        if (workerId < 0 || workerId > MAX_WORKER_ID)
        {
            throw std::invalid_argument("Worker ID must be between 0 and " +
                                        std::to_string(MAX_WORKER_ID));
        }
    }

    /**
     * Generate a unique snowflake ID
     */
    int64_t nextId()
    {
        std::lock_guard<std::mutex> lock(mutex);

        int64_t timestamp = currentTimeMillis();

        // Clock moved backwards - this is an error case
        if (timestamp < lastTimestamp)
        {
            throw std::runtime_error("Clock moved backwards. Refusing to generate ID");
        }

        // Same millisecond as last time - increment sequence
        if (timestamp == lastTimestamp)
        {
            sequence = (sequence + 1) & MAX_SEQUENCE;
            // Sequence exhausted in this millisecond - wait for next millisecond
            if (sequence == 0)
            {
                timestamp = waitNextMillis(timestamp);
            }
        }
        else
        {
            // New millisecond - reset sequence
            sequence = 0;
        }

        lastTimestamp = timestamp;

        // Compose the 64-bit ID from timestamp, worker ID, and sequence
        return ((timestamp - EPOCH) << TIMESTAMP_SHIFT) |
               (workerId << WORKER_ID_SHIFT) |
               sequence;
    }
};

#endif // SNOWFLAKE_HPP