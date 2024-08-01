from setuptools import setup

setup(
    name='zurox_lexer',
    packages=['.'],
    entry_points='''
    [pygments.lexers]
    zurox= zurox_lexer:ZuroxLexer
    '''
)
