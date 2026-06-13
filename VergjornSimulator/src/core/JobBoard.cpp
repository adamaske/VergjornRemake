#include "JobBoard.h"
#include <limits>
#include <cmath>

JobHandle JobBoard::postJob(const JobRecord& job) {
    JobRecord rec  = job;
    rec.handle.id  = nextId_++;
    rec.status     = EJobStatus::Open;
    jobs_[rec.handle.id] = rec;
    return rec.handle;
}

bool JobBoard::findBestJob(EOccupation occ, const Vec2& pos, JobHandle& out) const {
    const JobRecord* best = nullptr;
    float bestScore = -std::numeric_limits<float>::max();

    for (auto& [id, rec] : jobs_) {
        if (!rec.isOpen()) continue;
        // Idle means "any occupation"; otherwise must match
        if (rec.requiredOccupation != EOccupation::Idle &&
            rec.requiredOccupation != occ) continue;

        float dist  = pos.distTo(rec.location);
        float score = rec.priority * 1000.f - dist;
        if (score > bestScore || best == nullptr) {
            bestScore = score;
            best = &rec;
        }
    }

    if (best) { out = best->handle; return true; }
    return false;
}

bool JobBoard::claimJob(JobHandle h, uint32_t villagerId) {
    JobRecord* rec = getJobMut(h);
    if (!rec || !rec->isOpen()) return false;
    rec->status           = EJobStatus::Claimed;
    rec->claimedByVillager= villagerId;
    return true;
}

bool JobBoard::advanceJob(JobHandle h, float workUnits) {
    JobRecord* rec = getJobMut(h);
    if (!rec || rec->status == EJobStatus::Completed) return false;

    rec->status    = EJobStatus::InProgress;
    rec->workDone += workUnits;

    if (rec->workDone >= rec->workRequired) {
        rec->status = EJobStatus::Completed;
        if (onJobCompleted) onJobCompleted(*rec);
        return true;
    }
    return false;
}

void JobBoard::cancelJob(JobHandle h) {
    JobRecord* rec = getJobMut(h);
    if (rec) rec->status = EJobStatus::Cancelled;
}

const JobRecord* JobBoard::getJob(JobHandle h) const {
    auto it = jobs_.find(h.id);
    return it != jobs_.end() ? &it->second : nullptr;
}

JobRecord* JobBoard::getJobMut(JobHandle h) {
    auto it = jobs_.find(h.id);
    return it != jobs_.end() ? &it->second : nullptr;
}

int JobBoard::openJobCount() const {
    int n = 0;
    for (auto& [id, rec] : jobs_) if (rec.isOpen()) n++;
    return n;
}
