#pragma once
#include "Tags.h"
#include "MathTypes.h"
#include "Economy.h"
#include <unordered_map>
#include <functional>
#include <cstdint>
#include <string>

enum class EJobStatus : uint8_t { Open, Claimed, InProgress, Completed, Cancelled };

struct JobHandle {
    int32_t id = -1;
    bool isValid()    const { return id != -1; }
    void invalidate()       { id = -1; }
    bool operator==(const JobHandle& o) const { return id == o.id; }
};

struct JobRecord {
    JobHandle     handle;
    EJobType      type             = EJobType::Chop;
    EOccupation   requiredOccupation = EOccupation::Idle;  // Idle = any
    Vec2          location;
    int           priority         = 0;
    float         workRequired     = 10.f;
    float         workDone         = 0.f;
    EJobStatus    status           = EJobStatus::Open;
    uint32_t      claimedByVillager= 0;
    ReservationHandle linkedReservation;
    void*         targetEntity     = nullptr;  // opaque pointer to tree/building

    bool isOpen() const { return status == EJobStatus::Open; }
};

// Global job board. Buildings and resource nodes post jobs; villagers claim them.
// Arbitration: priority * 1000 - distance.
class JobBoard {
public:
    // Fires when a job completes (called inside AdvanceJob)
    std::function<void(const JobRecord&)> onJobCompleted;

    JobHandle postJob(const JobRecord& job);

    bool findBestJob(EOccupation occ, const Vec2& pos, JobHandle& out) const;

    bool claimJob(JobHandle h, uint32_t villagerId);

    // Returns true when job is complete. Broadcasts onJobCompleted.
    bool advanceJob(JobHandle h, float workUnits);

    void cancelJob(JobHandle h);

    const JobRecord* getJob(JobHandle h) const;
    JobRecord*       getJobMut(JobHandle h);

    int openJobCount() const;

private:
    std::unordered_map<int32_t, JobRecord> jobs_;
    int32_t nextId_ = 1;
};
