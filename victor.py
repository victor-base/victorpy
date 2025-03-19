from ctypes import *

try:
    lib = CDLL('./libvictor.so')
except OSError as e:
    print("ERROR: loading libvictor.so: %s" % e)
    exit(1)


# alloc_index signature in libvictor.so (view victor.h)
lib.alloc_index.argtypes = [c_int, c_int, c_int]
lib.alloc_index.restype = c_void_p

# insert signature in libvictor.so (view victor.h)
lib.insert.argtypes = [c_void_p, c_int, POINTER(c_float), c_int]
lib.insert.restype = c_int  

class VictorException(Exception):
    """Victor Exception"""

    ERROR_MESSAGES = {
        0: "SUCCESS",
        1: "INVALID_INIT",
        2: "INVALID_INDEX",
        3: "INVALID_VECTOR",
        4: "INVALID_RESULT",
        5: "INVALID_DIMENSIONS",
        6: "INVALID_ID",
        7: "INDEX_EMPTY",
        8: "SYSTEM_ERROR",
    }

    def __init__(self, error_code):
        self.error_code = error_code
        self.message = self.ERROR_MESSAGES.get(error_code, f"UNKNOWN_ERROR ({error_code})")
        super().__init__(self.message)

class Victor(object):
    def __init__(self, index_type, method, dims):
        self.index = lib.alloc_index(c_int(index_type), c_int(method), c_int(dims))
        if not self.index:
            raise VictorException(1)

    def insert(self, id, vec):
        array_type = c_float * len(vec)
        c_vec_ptr = cast(array_type(*vec), POINTER(c_float))
        result = lib.insert(self.index, c_int(id), c_vec_ptr, c_int(len(vec)))
        if result != 0:
            raise VictorException(result)


if __name__ == '__main__':
    index = Victor(0,0,4)
    index.insert(1, [1.0,2.0,3.0,4.0])


    