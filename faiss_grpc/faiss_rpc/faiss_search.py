# #cython: language_level=3
# import cython
import numpy as np
def faiss_search(vecs: List[List[float]], k:int, index:any) -> List[List[int]]:
    _, ind = index.search(np.array(vecs, dtype="float32"), k)
    return ind.tolist()