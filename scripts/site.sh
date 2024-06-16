. venv/bin/activate
cd docs_src/
mkdocs build
rm ../docs -rf
cp site ../docs -r
cd ..