#cp -r /home/obsidian/program/ljx /home/obsidian/program/git
cd ~/program/git
rm *.tgz
find ~/program/ljx -type f \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs tar czf ljx.tgz
find ~/virtualbox/xp/code -type f \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs tar czf xp.tgz
git add .
git commit -a -m "store"
git push -u origin master
