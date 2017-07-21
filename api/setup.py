import os
from setuptools import setup, find_packages
import pip
from pip.download import PipSession

def parse_requirements(reqs):
    return pip.req.parse_requirements(reqs, session=PipSession())

__version__ = None
exec(open('hpcpm/api/version.py').read())

SOLUTION_DIR = os.path.dirname(os.path.realpath(__file__))
REQUIREMENTS = parse_requirements(os.path.join(SOLUTION_DIR, 'requirements.txt'))

setup(name='hpc-power-management-api',
    version=__version__,
    description='HPC Power Management API',
    author='dev.lel.lu',
    author_email='mail.dev.lel.lu@gmail.com',
	url='dev.lel.lu',
    packages=find_packages(exclude=["*.tests", "*.tests.*", "tests.*", "tests"]),
    include_package_data=True,
    entry_points={
        'console_scripts': ['hpc-power-management-api=hpcpm.api.main:main']
    },
    install_requires=[str(ir.req) for ir in REQUIREMENTS],
    )
