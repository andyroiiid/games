//
// Created by Andrew Huang on 7/10/2022.
//

#include "system/job_system.h"

#include "logging.h"

JobSystem::JobSystem() {
    const unsigned int numThreads = std::max(1u, std::thread::hardware_concurrency());

    for (int i = 0; i < numThreads; ++i) {
        m_threads.emplace_back([this, i] { Worker(i); });
    }
}

JobSystem::~JobSystem() {
    m_running = false;
    m_pendingCV.notify_all();
    for (auto &thread: m_threads) {
        thread.join();
    }
}

void JobSystem::QueueJob(std::unique_ptr<Job> &&job) {
    {
        std::lock_guard<std::mutex> lk(m_pendingMutex);
        m_pending.push_back(std::move(job));
    }
    m_pendingCV.notify_one();
}

std::unique_ptr<Job> JobSystem::FetchPending() {
    std::unique_lock<std::mutex> lk(m_pendingMutex);
    m_pendingCV.wait(lk, [this] { return !m_pending.empty() || !m_running; });

    if (!m_running) {
        return nullptr;
    }

    std::unique_ptr<Job> job = std::move(m_pending.front());
    m_pending.pop_front();
    return job;
}

void JobSystem::QueueFinished(std::unique_ptr<Job> &&job) {
    std::lock_guard<std::mutex> lk(m_finishedMutex);
    m_finished.push_back(std::move(job));
}

std::unique_ptr<Job> JobSystem::FetchFinished() {
    std::lock_guard<std::mutex> lk(m_finishedMutex);

    if (m_finished.empty()) {
        return nullptr;
    }

    std::unique_ptr<Job> job = std::move(m_finished.front());
    m_finished.pop_front();
    return job;
}

void JobSystem::ProcessFinishedJobs() {
    // might want to add a time limit to this
    while (true) {
        std::unique_ptr<Job> job = FetchFinished();
        if (job) {
            job->Finish();
        } else {
            break;
        }
    }
}

void JobSystem::Worker(int i) {
    LogInfo("starting worker #%d\n", i);
    while (true) {
        std::unique_ptr<Job> job = FetchPending();
        if (job) {
            job->Execute();
            QueueFinished(std::move(job));
        } else {
            break;
        }
    }
    LogInfo("stopping worker #%d\n", i);
}
