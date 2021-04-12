import subprocess
import platform
from pathlib import Path
from setuptools.command.build_ext import build_ext
from distutils.core import setup, Extension


class CustomBuildExtension(build_ext):
    def build_extensions(self):
        # Override the compiler executables. Importantly, this
        # removes the "default" compiler flags that would
        # otherwise get passed on to to the compiler, i.e.,
        # distutils.sysconfig.get_var("CFLAGS").
        self.compiler.set_executable('compiler_so', 'g++')
        self.compiler.set_executable('compiler_cxx', 'g++')
        self.compiler.set_executable('linker_so', 'g++')
        build_ext.build_extensions(self)


def main():
    ext = Extension(
        'hsbg_sim',
        sources=[
            'src/enum_data.cpp', 'src/minion_events.cpp',
            'src/hero_powers.cpp', 'src/battle.cpp',
            'src/random.cpp', 'src/hsbgsimmodule.cpp'
        ],
        include_dirs=['./src'],
        extra_compile_args=['-std=c++11'] if platform.system() != 'Windows' else []
    )

    setup(name="hsbg_sim",
          version="1.0.0",
          description="Python interface for the Hearthstone Battlegrounds combat simulator.",
          author="Shon Verch",
          ext_modules=[ext],
          cmdclass={'build_ext': CustomBuildExtension}
    )

if __name__ == "__main__":
    subprocess.run(['python', 'scripts/generate_enum_data.py', 'hsdata/CardDefs.xml'])
    main()
