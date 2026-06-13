#include "Economy.h"
#include <algorithm>

bool Economy::tryReserve(EResource r, int amount, ReservationHandle& out) {
    ResourceLedger& e = entry(r);
    if (e.available() < amount) return false;

    ReservationHandle h;
    h.id       = nextId_++;
    h.resource = r;
    h.amount   = amount;
    e.reserved += amount;
    reservations_[h.id] = h;
    out = h;
    broadcast(r);
    return true;
}

void Economy::commitReservation(ReservationHandle& h) {
    if (!h.isValid()) return;
    auto it = reservations_.find(h.id);
    if (it == reservations_.end()) { h.invalidate(); return; }

    ResourceLedger& e = entry(h.resource);
    e.stored   -= h.amount;
    e.reserved -= h.amount;
    e.stored   = std::max(0, e.stored);
    e.reserved = std::max(0, e.reserved);
    reservations_.erase(it);
    broadcast(h.resource);
    h.invalidate();
}

void Economy::releaseReservation(ReservationHandle& h) {
    if (!h.isValid()) return;
    auto it = reservations_.find(h.id);
    if (it == reservations_.end()) { h.invalidate(); return; }

    ResourceLedger& e = entry(h.resource);
    e.reserved -= h.amount;
    e.reserved  = std::max(0, e.reserved);
    reservations_.erase(it);
    broadcast(h.resource);
    h.invalidate();
}

void Economy::deposit(EResource r, int amount) {
    if (amount <= 0) return;
    entry(r).stored += amount;
    broadcast(r);
}

bool Economy::tryWithdraw(EResource r, int amount) {
    ResourceLedger& e = entry(r);
    if (e.available() < amount) return false;
    e.stored -= amount;
    broadcast(r);
    return true;
}

int Economy::available(EResource r) const {
    const ResourceLedger* e = entryConst(r);
    return e ? e->available() : 0;
}

int Economy::stored(EResource r) const {
    const ResourceLedger* e = entryConst(r);
    return e ? e->stored : 0;
}

void Economy::markIncoming(EResource r, int amount) {
    entry(r).incoming += amount;
}

void Economy::unmarkIncoming(EResource r, int amount) {
    auto& e = entry(r);
    e.incoming = std::max(0, e.incoming - amount);
}

void Economy::broadcast(EResource r) {
    if (onResourceChanged) {
        const ResourceLedger& e = ledger_[static_cast<int>(r)];
        onResourceChanged(r, e.stored, e.available());
    }
}
