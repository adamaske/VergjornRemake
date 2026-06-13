#!/usr/bin/env python3
"""Render the ASCII world map as a colour image."""

import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from pathlib import Path

# ── Colour palette ────────────────────────────────────────────────────────────
COLOURS = {
    '.': (0.87, 0.82, 0.69),   # open ground — sandy
    'F': (0.22, 0.52, 0.22),   # forest zone — mid green
    'B': (0.38, 0.26, 0.12),   # bog zone — dark brown
    '~': (0.35, 0.65, 0.92),   # river — sky blue
    'G': (0.72, 0.85, 0.35),   # farmland — yellow-green
    'V': (0.68, 0.68, 0.68),   # village zone — grey
    '@': (0.90, 0.15, 0.15),   # village centre — red
    't': (0.04, 0.28, 0.04),   # tree node — dark green
    'o': (0.62, 0.28, 0.04),   # ore deposit — rust
    'f': (0.06, 0.32, 0.78),   # fishing spot — deep blue
}
DEFAULT = (0.87, 0.82, 0.69)

LEGEND_ITEMS = [
    ('F', 'Forest zone'),
    ('t', 'Tree'),
    ('B', 'Bog zone'),
    ('o', 'Ore deposit'),
    ('~', 'River'),
    ('f', 'Fishing spot'),
    ('V', 'Village zone'),
    ('@', 'Village centre'),
    ('.', 'Open ground'),
]


def parse_map(path: Path):
    """Parse map.txt → (grid rows, metadata lines)."""
    grid, meta = [], []
    in_grid = False
    with open(path) as fh:
        for raw in fh:
            line = raw.rstrip('\n')
            stripped = line.strip()
            if stripped.startswith('+--'):
                in_grid = not in_grid
                continue
            if in_grid and stripped.startswith('|'):
                grid.append(stripped[1:-1])   # strip surrounding |
            elif not in_grid and stripped:
                meta.append(stripped)
    return grid, meta


def render(grid, meta, out_path: Path):
    rows = len(grid)
    cols = max((len(r) for r in grid), default=0)
    if rows == 0 or cols == 0:
        print('Empty grid — nothing to render.')
        return

    img = np.zeros((rows, cols, 3))
    for r, row_str in enumerate(grid):
        for c, ch in enumerate(row_str):
            img[r, c] = COLOURS.get(ch, DEFAULT)
        # Pad short rows
        for c in range(len(row_str), cols):
            img[r, c] = DEFAULT

    fig, ax = plt.subplots(figsize=(14, 7))
    ax.imshow(img, aspect='auto', interpolation='nearest')
    ax.set_xticks([])
    ax.set_yticks([])

    # Border
    for spine in ax.spines.values():
        spine.set_edgecolor('#444444')
        spine.set_linewidth(1.5)

    # Legend
    patches = [
        mpatches.Patch(facecolor=COLOURS[ch], edgecolor='#333', linewidth=0.5, label=label)
        for ch, label in LEGEND_ITEMS
    ]
    ax.legend(handles=patches, loc='lower right', fontsize=8,
              framealpha=0.88, ncol=3, handlelength=1.2)

    # Title — first metadata line that contains "Vergjorn"
    title = next((m for m in meta if 'Vergjorn' in m), 'Vergjorn World Map')
    ax.set_title(title, fontsize=12, pad=8, fontweight='bold')

    # Info line from Zones/Nodes metadata
    info_parts = [m for m in meta if 'Zones' in m or 'Nodes' in m]
    if info_parts:
        ax.set_xlabel('  '.join(info_parts), fontsize=8, labelpad=6)

    plt.tight_layout()
    plt.savefig(out_path, dpi=150, bbox_inches='tight')
    plt.close()
    print(f'Saved {out_path}')


def main():
    map_file = Path(sys.argv[1]) if len(sys.argv) > 1 else Path('build/output/map.txt')
    out_file = map_file.with_suffix('.png')
    grid, meta = parse_map(map_file)
    render(grid, meta, out_file)


if __name__ == '__main__':
    main()
