from setuptools import setup
import numpy
from distutils.extension import Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext

extension = Extension(
           "faiss_search",
           sources=["faiss_grpc/faiss_rpc/faiss_search.py"],
           include_dirs=[numpy.get_include()],
           language="c++"
)
setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = cythonize(extension)
)