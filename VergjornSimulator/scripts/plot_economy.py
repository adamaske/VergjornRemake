#!/usr/bin/env python3
"""Plot resource economy over time."""
import sys
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

CSV = sys.argv[1] if len(sys.argv) > 1 else "output/stats.csv"

df = pd.read_csv(CSV)

resources = ["wood", "bog_iron", "charcoal", "iron_ingot",
             "tool_axe", "weapon_sword", "weapon_shield", "fish", "grain"]

# Only plot resources that ever have non-zero values
active = [r for r in resources if df[r].max() > 0]

fig, axes = plt.subplots(len(active), 1, figsize=(12, 3 * len(active)), sharex=True)
if len(active) == 1:
    axes = [axes]

for ax, res in zip(axes, active):
    ax.fill_between(df["day"], df[res], alpha=0.4)
    ax.plot(df["day"], df[res], linewidth=1.5, label=res.replace("_", " ").title())
    ax.set_ylabel("Stored")
    ax.legend(loc="upper left")
    ax.yaxis.set_major_locator(ticker.MaxNLocator(integer=True))
    ax.grid(True, alpha=0.3)

    # Season shading
    for _, row in df.drop_duplicates("day").iterrows():
        if row["season"] == "Winter":
            ax.axvspan(row["day"], row["day"] + 1,
                       color="lightblue", alpha=0.15, linewidth=0)

axes[-1].set_xlabel("Game Day")
fig.suptitle("Vergjorn — Resource Economy", fontsize=14, fontweight="bold")
plt.tight_layout()

out = CSV.replace(".csv", "_economy.png")
plt.savefig(out, dpi=150)
print(f"Saved {out}")
plt.show()
