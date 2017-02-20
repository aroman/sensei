### Installation

*NOTE:* This project uses Git submodules. Make sure you clone this repository with `--recursive`, like so:

```
$ git clone --recursive git@github.com:synergylabs/sensei.git
```

#### Dependencies

This project has a lot of dependencies. You can read `CMakeLists.txt` to see a full list.

##### Installing `mxnet`

We need to all be on the same version of mxnet, since the API can break.
0.9.3 is what we've settled on for now. To install that particular version:
```sh
cd ~
git clone --recursive --branch v0.9.3 https://github.com/dmlc/mxnet.git
cd mxnet
sh setup-utils/install-mxnet-osx-python.sh
```

*TODO*: List them here with a command to install all of them on macOS/Ubuntu

#### Building, running, debugging

**Build:** `./build.sh`
**Run:** `./run.sh`
**Debug:** `./run.sh --debug`

Simple!

#### Notes for Cyrus

1. You need to patch mxnet_mtcnn to make it single-threaded. This is trivially easy: edit `3rcParty/mxnet_mtcnn/mtcnn_detector.py` and on line 241 change `self.Pool.map` to `map`. NOTE: this may only be a problem on macOS.
