#!/usr/bin/env python3
"""Plot population and workforce over time."""
import sys
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

CSV = sys.argv[1] if len(sys.argv) > 1 else "output/stats.csv"

df = pd.read_csv(CSV)

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8), sharex=True)

# ── Top: population total ──────────────────────────────────────────────────────
ax1.plot(df["day"], df["population"], color="steelblue", linewidth=2, label="Population")
ax1.fill_between(df["day"], df["population"], alpha=0.2, color="steelblue")
ax1.set_ylabel("Villagers")
ax1.set_title("Population Growth")
ax1.legend()
ax1.grid(True, alpha=0.3)

# ── Bottom: working vs idle stacked area ───────────────────────────────────────
ax2.stackplot(df["day"],
              df["working"], df["idle"],
              labels=["Working", "Idle"],
              colors=["#2ecc71", "#e74c3c"],
              alpha=0.75)
ax2.set_ylabel("Villagers")
ax2.set_xlabel("Game Day")
ax2.set_title("Workforce Utilisation")
ax2.legend(loc="upper left")
ax2.grid(True, alpha=0.3)

# Mark season transitions
seasons = df.drop_duplicates(subset=["year", "season"])
for _, row in seasons.iterrows():
    for ax in (ax1, ax2):
        ax.axvline(row["day"], color="gray", linestyle="--", linewidth=0.6, alpha=0.5)
        ax.text(row["day"], ax.get_ylim()[1] * 0.95, row["season"][0],
                fontsize=7, color="gray", ha="center")

# Open jobs on secondary axis
ax3 = ax2.twinx()
ax3.plot(df["day"], df["open_jobs"], color="purple", linewidth=1,
         linestyle=":", label="Open Jobs")
ax3.set_ylabel("Open Jobs", color="purple")
ax3.tick_params(axis="y", labelcolor="purple")
ax3.legend(loc="upper right")

fig.suptitle("Vergjorn — Population & Workforce", fontsize=14, fontweight="bold")
plt.tight_layout()

out = CSV.replace(".csv", "_population.png")
plt.savefig(out, dpi=150)
print(f"Saved {out}")
plt.show()
