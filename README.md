# pytorch-uvm

## Project Overview

This project introduces the NVIDIA UVM (Unified Virtual Memory) mechanism to PyTorch, enabling model training or inference that would otherwise fail due to Out Of Memory (OOM) errors. This allows large models to run on systems with insufficient GPU memory.

## Technical Principle

By leveraging the LD_PRELOAD mechanism in Linux, we intercept the CUDA memory operations APIs used by PyTorch and replace them with UVM-capable APIs that allow the use of Unified Virtual Memory (UVM).

## Compilation and Installation

### Dependencies

Before starting, make sure your environment has the following dependencies installed:

- Python 3.8
- PyTorch 2.7 (or higher)
- CUDA toolchain

### Installation

1. Clone the project to your local machine:
   ```bash
   git clone https://github.com/csc010228/pytorch-uvm.git
   cd pytorch-uvm
   ```

2. Compile the project:
   ```bash
   make
   ```

### Usage

```bash
LD_PRELOAD=ptuvm.so python3 your_model_training_or_inference_script.py
```

## TODO List

- [ ] Test more models and check whether the CUDA operation API is not blocked.
- [ ] Introduce prefetching operations to speed up the training