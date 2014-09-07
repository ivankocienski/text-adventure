" Vim syntax file
" Language: Adventure world file
" Maintainer: Me
" Latest Revision: 5th September 2014

syn keyword worldCommand NAME AUTHOR COPYRIGHT INCLUDE START 
syn keyword worldCommand ROOM DESCRIBE ENDDESCRIBE EXIT ENDROOM ITEM

syn match worldComment "#.*$"

let b:current_syntax = "worldfile"

hi def link worldCommand Statement
hi def link worldComment Comment
