# VictorPy

VictorPy provides **Python bindings** for **libvictor**, a lightweight and fast vector database engine written in C. This package allows you to build, search, and manage vector indices directly from Python with minimal overhead.

---

## Features

- **Flat** and **NSW (Navigable Small World)** indices.
- **Cosine**, **L2 Norm (Euclidean)**, and **Dot Product** similarity metrics.
- Insert, search, remove vectors.
- Dump/load index from disk.
- Lightweight, high-performance core in C.
- Minimal external dependencies (only `pybind11`).

---

## Installation

**Requirements:**
- Python >= 3.7
- C++17 compiler
- [libvictor](https://github.com/yourrepo/libvictor) installed on your system

```bash
pip install victorpy
```

If building from source:

```bash
git clone https://github.com/yourrepo/victorpy.git
cd victorpy
python setup.py install
```

Make sure `libvictor.a` (or `libvictor.dylib` on macOS) is in your system library path (or use `LD_LIBRARY_PATH` / `DYLD_LIBRARY_PATH`).

---

## Basic Usage

```python
from victor import VictorIndex, COSINE, NSW_INDEX

# Create a new index
index = VictorIndex(NSW_INDEX, COSINE, 512)

# Insert 1000 random vectors
import numpy as np
for i in range(1000):
    vec = np.random.rand(512).astype(np.float32)
    index.insert(i, vec)

# Search for a random vector
query = np.random.rand(512).astype(np.float32)
id, distance = index.search(query)
print(f"Best match ID: {id}, distance: {distance}")

# Dump the index to disk
index.dump("index_file.victor")
```

---

## API Reference

### Constants
- `COSINE` : Cosine similarity
- `L2NORM` : L2 (Euclidean) distance
- `DOTP`   : Dot product
- `FLAT_INDEX` : Flat (brute-force) index
- `NSW_INDEX`  : Navigable Small World graph

### VictorIndex

```python
VictorIndex(type: int, method: int, dims: int)
VictorIndex.load(filename: str) -> VictorIndex
```

**Methods:**
- `insert(id: int, vector: List[float])`
- `search(vector: np.ndarray) -> Tuple[int, float]`
- `search_n(vector: np.ndarray, n: int) -> List[Tuple[int, float]]`
- `remove(id: int)`
- `contains(id: int) -> bool`
- `size() -> int`
- `dump(filename: str)`
- `stats() -> dict`

---

## License

This project is licensed under the **LGPLv3** license.


## Contact

Author: Emiliano Billi  
Email: [emiliano.billi@gmail.com](mailto:emiliano.billi@gmail.com)

---