#!/usr/bin/env python3

import server
import sys

if __name__ == "__main__":
    if len(sys.argv) < 2:
        server.run()
    else:
        server.run(sys.argv[1])
