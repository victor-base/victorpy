from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import pybind11

ext_modules = [
    Extension(
        "victor.victor",
        ["victor/binding.cpp"],
        include_dirs=[
            pybind11.get_include(),
            "/usr/local/include",
        ],
        extra_objects=["/usr/local/lib/libvictor.a"],
        language="c++",
        extra_compile_args=["-O3", "-std=c++17"],
    )
]

setup(
    name="victor",
    version="0.1.0",
    author="Emiliano Billi",
    author_email="emiliano.billi@gmail.com",
    description="Python bindings for libvictor (vector database engine)",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    url="",
    packages=["victor"],
    ext_modules=ext_modules,
    install_requires=["pybind11"],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: LGPLv3 License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.7',
)
