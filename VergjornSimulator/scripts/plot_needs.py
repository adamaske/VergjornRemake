#!/usr/bin/env python3
"""Plot villager needs (hunger, warmth, rest, morale) over time."""
import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

CSV = sys.argv[1] if len(sys.argv) > 1 else "output/stats.csv"

df = pd.read_csv(CSV)

needs      = ["avg_hunger", "avg_warmth", "avg_rest", "avg_morale"]
labels     = ["Hunger",     "Warmth",     "Rest",     "Morale"]
colors     = ["#e67e22",    "#3498db",    "#9b59b6",  "#27ae60"]

fig, axes = plt.subplots(2, 2, figsize=(14, 8), sharex=True)
axes = axes.flatten()

for ax, col, label, color in zip(axes, needs, labels, colors):
    ax.plot(df["day"], df[col], color=color, linewidth=1.8, label=label)
    ax.fill_between(df["day"], df[col], alpha=0.15, color=color)
    ax.axhline(0.5, color="red", linestyle="--", linewidth=0.8, alpha=0.6, label="Critical (0.5)")
    ax.set_ylim(0, 1.05)
    ax.set_ylabel("Score [0–1]")
    ax.set_title(label)
    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3)

    # Shade winters
    winter_rows = df[df["season"] == "Winter"]
    if not winter_rows.empty:
        starts = winter_rows["day"].values
        for start in starts:
            ax.axvspan(start, start + 30, color="lightblue", alpha=0.2, linewidth=0)

for ax in axes[2:]:
    ax.set_xlabel("Game Day")

fig.suptitle("Vergjorn — Average Villager Needs", fontsize=14, fontweight="bold")
plt.tight_layout()

out = CSV.replace(".csv", "_needs.png")
plt.savefig(out, dpi=150)
print(f"Saved {out}")
plt.show()
