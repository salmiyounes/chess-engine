from setuptools import setup
from setuptools.command.build_py import build_py
import subprocess
import sisyphus
import os

class CustomBuildPyCommand(build_py):
    def run(self):
        subprocess.check_call(['make'], cwd='sisyphus')
        super().run()

setup(
    name='sisyphus',
    version=sisyphus.__version__,
    author=sisyphus.__author__,
    description='Sisyphus chess engine',
    packages=['sisyphus'],
    package_dir={'sisyphus': 'sisyphus'},
    package_data={'sisyphus': ['libchess.so']},  
    cmdclass={'build_py': CustomBuildPyCommand},
)
