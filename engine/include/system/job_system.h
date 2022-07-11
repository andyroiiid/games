//
// Created by Andrew Huang on 7/10/2022.
//

#ifndef GAMES_JOB_SYSTEM_H
#define GAMES_JOB_SYSTEM_H

#include <thread>
#include <mutex>
#include <vector>
#include <deque>

#include "non_copyable.h"

class Job : NonCopyable {
public:
    Job() = default;

    virtual ~Job() = default;;

    // run in worker thread
    virtual void Execute() = 0;

    // run in main thread
    virtual void Finish() = 0;
};


class JobSystem : NonCopyable {
public:
    JobSystem();

    // 1. finish currently executing jobs
    // 2. ignore remaining jobs
    ~JobSystem();

    void QueueJob(std::unique_ptr<Job> &&job);

    void ProcessFinishedJobs();

private:
    std::unique_ptr<Job> FetchPending();

    void QueueFinished(std::unique_ptr<Job> &&job);

    std::unique_ptr<Job> FetchFinished();

    void Worker(int i);

    // workers
    std::vector<std::thread>         m_threads;
    std::atomic<bool>                m_running = true;
    // job queue
    std::mutex                       m_pendingMutex;
    std::condition_variable          m_pendingCV;
    std::deque<std::unique_ptr<Job>> m_pending;
    // finished queue
    std::mutex                       m_finishedMutex;
    std::deque<std::unique_ptr<Job>> m_finished;
};

#endif //GAMES_JOB_SYSTEM_H
