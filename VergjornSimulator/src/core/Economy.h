#pragma once
#include "Tags.h"
#include <unordered_map>
#include <functional>
#include <cstdint>

struct ResourceLedger {
    int stored   = 0;
    int reserved = 0;
    int incoming = 0;

    int available() const { return stored - reserved; }
};

struct ReservationHandle {
    int32_t id      = -1;
    EResource resource = EResource::COUNT;
    int       amount   = 0;

    bool isValid() const { return id != -1; }
    void invalidate()    { id = -1; }
};

// Pure-logic resource ledger with reservation semantics.
// Stored - Reserved = Available. Haul jobs operate against reservations
// so two sites can never claim the same stockpile.
class Economy {
public:
    // Callbacks
    std::function<void(EResource, int stored, int available)> onResourceChanged;

    bool tryReserve(EResource r, int amount, ReservationHandle& out);
    void commitReservation(ReservationHandle& h);   // materials consumed
    void releaseReservation(ReservationHandle& h);  // reservation cancelled

    void deposit(EResource r, int amount);
    bool tryWithdraw(EResource r, int amount);

    int available(EResource r) const;
    int stored(EResource r)    const;

    void markIncoming(EResource r, int amount);
    void unmarkIncoming(EResource r, int amount);

    const std::unordered_map<int, ResourceLedger>& ledger() const { return ledger_; }

private:
    // Key is static_cast<int>(EResource)
    std::unordered_map<int, ResourceLedger> ledger_;
    int32_t nextId_ = 1;
    std::unordered_map<int32_t, ReservationHandle> reservations_;

    ResourceLedger& entry(EResource r) {
        return ledger_[static_cast<int>(r)];
    }
    const ResourceLedger* entryConst(EResource r) const {
        auto it = ledger_.find(static_cast<int>(r));
        return it != ledger_.end() ? &it->second : nullptr;
    }

    void broadcast(EResource r);
};
