# Contributing Guidelines

Thanks for your interest in contributing! 
Please take a moment to read through these guidelines to keep things smooth.

---

## Getting Started

1. **Fork** the repository and **clone** it locally.
2. Install dependencies:
- [Bear](https://github.com/rizsotto/Bear)
- libjansson
- [fzf](https://github.com/junegunn/fzf)
3. Run tests to confirm everything works:
```sh
cd test-make
make clean
virt-pack -- make
virt-pack --info
virt-pack --remove
```

---

## How to Contribute

### Reporting Issues
- Use the [issue tracker](../../issues).
- Check for existing issues before creating a new one.
- Include steps to reproduce, expected behavior, and screenshots if relevant.

### Submitting Changes
1. Create a new branch:
   git checkout -b feature/your-feature-name
2. Make your changes.
3. Run tests/linters before committing:
```sh
cd test-make
make clean
virt-pack -- make
virt-pack --show
virt-pack --remove
```
4. Commit with a meaningful message:
   git commit -m "Add: short description of change"
5. Push and open a Pull Request (PR) to the `main` branch.

---

## Coding Standards
- Follow the existing code style.
- Add comments where needed.
- Write tests for new features or fixes.

---

## Pull Request Guidelines
- Keep PRs small and focused.
- Link related issue numbers (`Fixes #123`).
- Ensure your branch is up-to-date with `main`.

---

## Community
- Be respectful and constructive.

---

## Recognition
Contributions count towards [Hacktoberfest](https://hacktoberfest.com/)! 🌱  
Your PRs will be labeled and reviewed accordingly.
