from setuptools import setup
from setuptools.command.build_py import build_py
import subprocess
import os

class CustomBuildPyCommand(build_py):
    def run(self):
        subprocess.check_call(['make'], cwd='sisyphus')
        super().run()

setup(
    name='sisyphus',
    version='0.1.0',
    description='Python chess engine',
    packages=['sisyphus'],
    package_dir={'sisyphus': 'sisyphus'},
    package_data={'sisyphus': ['libchess.so']},  
    cmdclass={'build_py': CustomBuildPyCommand},
)
