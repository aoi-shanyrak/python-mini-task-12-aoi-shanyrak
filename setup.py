from setuptools import Extension, setup

setup(
    name="power_matrix",
    version="1.0.0",
    description="In Python powering by number matrix as list with lists of floats from C",
    author="aoi-shanyrak",
    ext_modules=[
        Extension(
            name="power_matrix",
            sources=["power_matrix.c"]
        )
    ]
)