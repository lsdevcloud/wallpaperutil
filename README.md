# **wallpaperutil**

A simple C program that downloads a wallpaper from a given URL, saves it, and creates a symlink to the latest wallpaper.

Linux **ONLY** btw

## **Default Behavior**

By default, the wallpaper is saved to:
`~/.local/share/wallpapers/[date].png`

A symlink to the latest downloaded wallpaper is created at:
`~/.local/share/wallpapers/latest.png`

## **Usage**

```sh
wallpaperutil [-o <output_path>] [-s <symlink_path>] <image_url>
```

### **Flags:**

- `-o <output_path>` → Specifies the destination for the downloaded image.
- `-s <symlink_path>` → Specifies the destination for the symlink to the downloaded image.

## **Compilation**

Clone this repository and compile the program with:

```sh
gcc -o wallpaperutil main.c -lcurl
```

## **Installation (optional)**

To use `wallpaperutil` globally, copy the binary to `/usr/bin/` (requires root privileges):

```sh
sudo cp wallpaperutil /usr/bin/
```

Alternatively, for a user-specific installation:

```sh
mkdir -p ~/.local/bin
cp wallpaperutil ~/.local/bin/
export PATH=$HOME/.local/bin:$PATH  # Add this to ~/.bashrc or ~/.zshrc for persistence
```

wallpaperutil © 2025 by lsdevcloud is licensed under CC BY-NC-SA 4.0 (see `LICENSE`)
