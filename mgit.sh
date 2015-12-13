cp /home/obsidian/program/ljx /home/obsidian/program/git
find /home/obsidian/program/git/ljx -type f -not \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) -exec rm {} \;
git add .
git commit -a -m "store"
git push -u origin master
