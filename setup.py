from distutils.core import setup, Extension
from pathlib import Path

def main():
    ext = Extension(
        "hsbg_sim",
        sources=['src/enum_data.cpp', 'src/minion_events.cpp', 'src/hero_powers.cpp', 'src/battle.cpp', 'src/random.cpp', 'src/hsbgsimmodule.cpp'],
        include_dirs=['./src']
    )
    setup(name="hsbg_combat_sim",
          version="1.0.0",
          description="Python interface for the Hearthstone Battlegrounds combat simulator.",
          author="Shon Verch",
          ext_modules=[ext])

if __name__ == "__main__":
    main()
