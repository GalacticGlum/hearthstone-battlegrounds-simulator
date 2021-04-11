import subprocess
from pathlib import Path
from distutils.core import setup, Extension
from distutils.command.install import install as DistutilsInstall


class CustomInstall(DistutilsInstall):
    """A custom installer for this package."""
    def run(self):
        # Generate the enum data
        subprocess.run(['python', 'scripts/generate_enum_data.py', 'hsdata/CardDefs.xml'])
        super().run()

def main():
    ext = Extension(
        "hsbg_sim",
        sources=[
            'src/enum_data.cpp', 'src/minion_events.cpp',
            'src/hero_powers.cpp', 'src/battle.cpp',
            'src/random.cpp', 'src/hsbgsimmodule.cpp'
        ],
        include_dirs=['./src']
    )
    setup(name="hsbg_sim",
          version="1.0.0",
          description="Python interface for the Hearthstone Battlegrounds combat simulator.",
          author="Shon Verch",
          ext_modules=[ext],
          cmdclass={'install': CustomInstall}
    )

if __name__ == "__main__":
    main()
