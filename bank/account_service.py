import os
import sqlite3

_DB = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bank.db')

def get_accounts(owner):
    # Parameterized query prevents SQL injection — no string interpolation ever touches the DB
    try:
        con = sqlite3.connect(_DB)
        cur = con.cursor()
        cur.execute('SELECT id FROM accounts WHERE owner=?', (owner,))
        return [row[0] for row in cur.fetchall()]
    finally:
        con.close()

def get_balance(account_number, owner):
    # owner parameter enforces authorization — a user can only read their own balance
    # Parameterized query prevents SQL injection
    try:
        con = sqlite3.connect(_DB)
        cur = con.cursor()
        cur.execute('''
            SELECT balance FROM accounts where id=? and owner=?''',
            (account_number, owner))
        row = cur.fetchone()
        if row is None:
            return None
        return row[0]
    finally:
        con.close()

def do_transfer(source, target, amount):
    # Both UPDATEs run before commit — if either fails, the connection closes
    # without committing, so SQLite auto-rolls back
    # Parameterized queries on all three statements prevent SQL injection
    try:
        con = sqlite3.connect(_DB)
        cur = con.cursor()
        cur.execute('''
            SELECT id FROM accounts where id=?''',
            (target,))
        row = cur.fetchone()
        if row is None:
            return False
        cur.execute('''
            UPDATE accounts SET balance=balance-? where id=?''',
            (amount, source))
        cur.execute('''
            UPDATE accounts SET balance=balance+? where id=?''',
            (amount, target))
        con.commit()
        return True
    finally:
        con.close()