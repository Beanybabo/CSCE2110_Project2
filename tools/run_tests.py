#!/usr/bin/env python3
# run_tests.py
# Purpose: Runs ./campus with scripted menu input on data/ and testdata/,
#          checks the real output, and writes test_results.txt.
#          Nothing is hard-coded: every "Actual" line is captured from the
#          program, and PASS/FAIL comes from comparing it to expected text.
# Usage:   make && python3 tools/run_tests.py     (from the project root)
import subprocess, re, sys
def run(d, inputs):
    p = subprocess.run(['./campus', d], input="\n".join(inputs+['0'])+"\n", capture_output=True, text=True, timeout=10)
    out = p.stdout.replace('Enter your choice: ','')
    parts = out.split('===== Campus Menu =====')
    load = [l for l in parts[0].split('\n') if l.strip()]
    ops=[]
    for part in parts[1:]:
        for l in part.split('\n'):
            if l.strip() and l.strip()!='Goodbye.' and not re.match(r'^\d\. ', l):
                ops.append(l)
    if 'Goodbye.' in out: ops.append('(program exited normally: Goodbye.)')
    return p.returncode, load, ops
T=[]
def t(desc, d, inputs, expect, checks, show='ops', inp=None):
    T.append(dict(desc=desc,d=d,inputs=inputs,expect=expect,checks=checks,show=show,inp=inp))
S='data'
# ---- file loading
t('Sample files load: valid records, blank lines, comments',S,[],'4 summaries: buildings 7/0, connections 6/0, facilities 10/0 (capacity 13, entries 10, collisions 1), departments 7/0',['buildings.txt: 7 accepted, 0 rejected','connections.txt: 6 accepted, 0 rejected','facilities.txt: 10 accepted, 0 rejected (hash capacity 13, entries 10, collisions 1)','departments.txt: 7 accepted, 0 rejected'],'load')
t('Too few / too many fields (messy test files)','testdata/messy',[],'Warnings with filename, line number, reason: connections lines 6,7; facilities lines 6,8; departments line 5',['connections.txt line 6: expected 2 fields','connections.txt line 7: expected 2 fields','facilities.txt line 6: expected 4 fields','facilities.txt line 8: expected 4 fields','departments.txt line 5: expected 2 fields'],'load')
t('Missing file (facilities.txt absent)','testdata/missing_files',[],'Error naming facilities.txt, treated as empty (0/0), program continues to the menu',['Error: could not open facilities.txt','facilities.txt: 0 accepted, 0 rejected'],'load')
t('Duplicate building (capitalization) and duplicate facility ID (capitalization)','testdata/messy',[],'buildings line 5 "library" rejected; facilities line 2 "f001" rejected, first record kept',['buildings.txt line 5: duplicate building','buildings.txt: 4 accepted, 1 rejected','facilities.txt line 2: duplicate facility ID F001'],'load')
t('Unknown building refs, duplicate and reversed connections, unknown department parent','testdata/messy',[],'connections line 2 (reversed duplicate) and line 4 (Mars Hall) rejected; facilities line 3 unknown building; departments line 4 unknown parent',['connections.txt line 2: duplicate connection','connections.txt line 4: unknown building','facilities.txt line 3: unknown building','departments.txt line 4: unknown parent'],'load')
t('[Final] Self-connection and duplicate department differing by capitalization','testdata/messy',[],'connections line 3 self-connection rejected; departments line 3 duplicate rejected',['connections.txt line 3: self-connection','departments.txt line 3: duplicate department'],'load')
t('[Final] Empty files','testdata/empty_files',[],'All four load as 0 accepted, 0 rejected, no crash',['buildings.txt: 0 accepted, 0 rejected','facilities.txt: 0 accepted, 0 rejected','departments.txt: 0 accepted, 0 rejected'],'load')
t('[Final] Windows line endings, extra whitespace, lowercase ID, # inside a name','testdata/crlf',['1'],'All records accepted (0 rejected); f001 stored as F001; "Main Room #5" kept; whitespace trimmed',['buildings.txt: 3 accepted, 0 rejected','connections.txt: 2 accepted, 0 rejected','facilities.txt: 2 accepted, 0 rejected','F001 | Main Room #5 | Library | Library','F002 | Study Lounge | Gym | Lounge'],'both')
# ---- hash and search
t('View all facilities (file order + total)',S,['1'],'10 facilities F001..F010 in file order, then Total facilities: 10',['F001 | Main Reading Room | Library | Library','F010 | Research Help Desk | Library | Library','Total facilities: 10']) 
t('View facilities when none are loaded','testdata/empty_files',['1'],'Clear empty message',['No facilities are loaded.'])
t('ID search: F004 (found)',S,['2','F004'],'Found: Computer Lab A | Science Building | Computer Lab',['Found: F004 | Computer Lab A | Science Building | Computer Lab'])
t('ID search: f004 (lowercase)',S,['2','f004'],'Same result as F004',['Found: F004 | Computer Lab A | Science Building | Computer Lab'])
t('ID search: 004 (invalid format)',S,['2','004'],'Invalid format message',['Invalid format: an ID is F followed by exactly three digits.'])
t('ID search: F999 (valid, not found)',S,['2','F999'],'Not found message with normalized ID F999',['Not found: no facility with ID F999'])
t('ID search: f999 (lowercase not found shows normalized ID)',S,['2','f999'],'Not found message shows F999',['Not found: no facility with ID F999'])
t('Hash collision + duplicate-key attempt (collision test data)','testdata/collision',['1'],'F005 and F010 share bucket 12: collisions 1, entries 2. Duplicate f005 rejected and collision count stays 1',['facilities.txt line 5: duplicate facility ID F005','facilities.txt: 2 accepted, 1 rejected (hash capacity 13, entries 2, collisions 1)','Total facilities: 2'],'both')
# ---- graph / BFS
t('View campus graph',S,['3'],'Buildings in file order, neighbors in connection order; Observatory shows (no connections)',['Library: Student Union, Business Building','Student Union: Library, Science Building, Gym','Observatory: (no connections)'])
t('Graph counts and degrees',S,['4'],'7 buildings, 6 connections, degrees 2,3,2,1,2,2,0',['Total buildings: 7','Total connections: 6','Library: 2','Student Union: 3','Engineering Building: 1','Observatory: 0'])
t('BFS from a connected building (Library)',S,['5','Library'],'Library 0, Student Union 1, Business Building 1, Science Building 2, Gym 2, Engineering Building 3; 6 of 7',['Library (Distance: 0','Student Union (Distance: 1','Business Building (Distance: 1','Science Building (Distance: 2','Gym (Distance: 2','Engineering Building (Distance: 3','Reachable buildings: 6 out of 7'])
t('BFS from an isolated building (Observatory)',S,['5','observatory'],'Only Observatory, 1 of 7 (also tests lowercase input)',['Observatory (Distance: 0','Reachable buildings: 1 out of 7'])
t('BFS with an unknown start',S,['5','Mars Hall'],'Error: building not found, returns to menu',['Error: Building not found.'])
t('BFS with a blank name',S,['5',''],'Blank-name error, returns to menu',['Error: the building name cannot be blank.'])
t('Graph with multiple connected components (messy data): counts',"testdata/messy",['4'],'4 buildings, 2 connections',['Total buildings: 4','Total connections: 2'])
t('Multiple components: BFS from Library reaches only its component',"testdata/messy",['5','Library'],'Library, Student Union; 2 of 4',['Library (Distance: 0','Student Union (Distance: 1','Reachable buildings: 2 out of 4'])
t('Empty graph (no buildings): display and counts','testdata/empty_files',['3','4'],'Clear empty message, zero totals',['There are no buildings in the campus graph.','Total buildings: 0'])
# ---- tree
t('Empty tree: add root without a parent prompt','testdata/empty_files',['6','Root Dept','8'],'Added as root with no parent question; hierarchy shows Root Dept',["Added 'Root Dept' as the root department.",'Root Dept'])
t('Root-only tree: search root and preorder','testdata/root_only',['7','Only Department','9','8'],'Parent None, Children 0, Depth 0; preorder and hierarchy show one department',['Name: Only Department','Parent: None','Children: 0','Depth: 0'])
t('Search root department',S,['7','University'],'Parent None, Children 2, Depth 0',['Name: University','Parent: None','Children: 2','Depth: 0'])
t('Search non-root department (lowercase, spaces)',S,['7','computer science'],'Parent College of Engineering, Children 0, Depth 2',['Name: Computer Science','Parent: College of Engineering','Children: 0','Depth: 2'])
t('Search missing department',S,['7','Chemistry'],'Not found message',['Department could not be found.'])
t('Add department with unknown parent',S,['6','Chemistry','Nowhere','8'],'Error naming the parent; tree unchanged',["Error: parent department 'Nowhere' does not exist."])
t('Add duplicate department (different capitalization)',S,['6','physics'],'Error: already exists; no parent prompt',["Error: department 'physics' already exists."])
t('Add department with valid parent, then hierarchy',S,['6','Chemistry','College of Science','8'],'Chemistry added under College of Science; hierarchy shows it indented 8 spaces',["Added 'Chemistry' under 'College of Science'.",'        Chemistry'])
t('Display department hierarchy (4-space indent)',S,['8'],'Sample hierarchy, 4 spaces per depth',['University','    College of Engineering','        Computer Science','    College of Science','        Physics'])
t('Preorder traversal',S,['9'],'Sequence without indentation in preorder',['University -> College of Engineering -> Computer Science -> Electrical Engineering -> College of Science -> Mathematics -> Physics'])
t('Preorder and hierarchy on an empty tree','testdata/empty_files',['8','9'],'Clear empty message (twice)',['There are no departments found.'])
# ---- menu
t('Invalid menu input: letters, blank, out of range, negative',S,['abc','','99','-1'],'Error each time, menu returns',['Error: please enter a number from the menu (0-9).'])
t('Blank names rejected: add department, search department, facility ID',S,['6','','7','','2',''],'Blank department name errors; blank ID reported as invalid format',['Error: the department name cannot be blank.','Invalid format'])
t('Repeated operations after errors',S,['99','2','F004','abc','2','F001','5','nowhere','5','Gym'],'Each valid operation works after an error',['Found: F004','Found: F001 | Main Reading Room','Error: Building not found.','Gym (Distance: 0'])
t('Exit cleanly with option 0',S,[],'Goodbye and normal exit',['Goodbye.'],'ops')

for i,x in enumerate(T,1):
    rc,load,ops=run(x['d'],x['inputs'])
    allout="\n".join(load+ops)
    x['rc']=rc; x['load']=load; x['ops']=ops
    x['ok']= rc==0 and all(c in allout for c in x['checks'])
    print(i, 'PASS' if x['ok'] else 'FAIL', x['desc'])

def cut(lines,n=22): return lines if len(lines)<=n else lines[:n]+['...']
L=["TEST RESULTS - Campus Data Explorer (Milestone 1)","="*60,
"Build: g++ -std=c++11 -g -Iinclude src/*.cpp -o campus",
"Run:   ./campus [data-directory]   (default: data)",
"Generated by: python3 tools/run_tests.py (output below is captured, not typed)",
"Tests marked [Final] are extra checks beyond the Milestone 1 list.",
"Each test uses scripted menu input; 'Actual' is the program output",
"(menu text removed, long output shortened with ...).",
"CELL run: [TEAM: record date and result here]",""]
for i,x in enumerate(T,1):
    L+=["-"*60,"Test %d: %s"%(i,x['desc']),"Data directory: "+x['d']]
    seq=x['inputs']
    L.append("Menu input: "+(" , ".join("(blank)" if s=='' else s for s in seq)+" , 0" if seq else "0 (just exit)"))
    L+=["Expected: "+x['expect'],"Actual:"]
    show = x['load'] if x['show']=='load' else (x['load']+x['ops'] if x['show']=='both' else (x['ops'] or x['load']))
    L+=["    "+l for l in cut(show)]
    L.append("Result: "+("PASS" if x['ok'] else "FAIL"))
L+=["-"*60,"SUMMARY: %d of %d tests passed."%(sum(x['ok'] for x in T),len(T))]
open('test_results.txt','w').write("\n".join(L)+"\n")
print("Wrote test_results.txt")
