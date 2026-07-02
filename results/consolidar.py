import os
import re
import csv
from collections import defaultdict

PASTA = "."
SAIDA = "consolidado.csv"

def extrair_valor(cell):
    """Extrai o número antes do parêntese, ex: '8.00(48/6)' -> 8.00, '11' -> 11"""
    cell = cell.strip()
    match = re.match(r'^([\d.]+)', cell)
    if match:
        return float(match.group(1))
    return None

# Detecta n e d a partir do nome da pasta/arquivo
def extrair_nd(nome_arquivo):
    """Tenta extrair n e d do nome do CSV (ex: n100-d20.csv ou n100d20.csv)"""
    match = re.search(r'n(\d+)[_-]?d(\d+)', nome_arquivo)
    if match:
        return int(match.group(1)), int(match.group(2))
    return None, None

colunas = [
    'Simples','Simples_CG','Simples_F','Simples_F_CG',
    'LF','LF_CG','LF_F','LF_F_CG',
    'SL','SL_CG','SL_F','SL_F_CG',
    'DSATUR','DSATUR_CG','DSATUR_F','DSATUR_F_CG'
]

resultados = []

for nome_csv in sorted(os.listdir(PASTA)):
    if not nome_csv.endswith('.csv'):
        continue

    n, d = extrair_nd(nome_csv)
    if n is None:
        print(f"Ignorando {nome_csv} — não consegui extrair n e d do nome")
        continue

    caminho = os.path.join(PASTA, nome_csv)
    medias = defaultdict(list)

    with open(caminho, newline='', encoding='utf-8') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            for col in colunas:
                val = extrair_valor(row.get(col, ''))
                if val is not None:
                    medias[col].append(val)

    linha = {'n': n, 'd': d}
    for col in colunas:
        vals = medias[col]
        linha[col] = round(sum(vals) / len(vals), 2) if vals else ''

    resultados.append(linha)
    print(f"OK: {nome_csv} — n={n}, d={d}, instâncias={len(medias[colunas[0]])}")

# Ordena por n e d
resultados.sort(key=lambda x: (x['n'], x['d']))

# Escreve o CSV consolidado
with open(SAIDA, 'w', newline='', encoding='utf-8') as f:
    fieldnames = ['n', 'd'] + colunas
    writer = csv.DictWriter(f, fieldnames=fieldnames, delimiter=';')
    writer.writeheader()
    writer.writerows(resultados)

print(f"\nConsolidado gerado: {SAIDA} ({len(resultados)} configurações)")