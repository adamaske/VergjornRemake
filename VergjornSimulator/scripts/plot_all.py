#!/usr/bin/env python3
"""Run all plots against a stats CSV."""
import subprocess, sys, pathlib

csv = sys.argv[1] if len(sys.argv) > 1 else "output/stats.csv"
scripts_dir = pathlib.Path(__file__).parent

for script in ["plot_economy.py", "plot_population.py", "plot_needs.py"]:
    subprocess.run([sys.executable, str(scripts_dir / script), csv], check=True)
