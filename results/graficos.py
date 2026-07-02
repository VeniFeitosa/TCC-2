import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import numpy as np
import os

df = pd.read_csv('consolidado.csv', sep=';')

os.makedirs('graficos', exist_ok=True)

cols_frac = ['Simples_F','Simples_F_CG','LF_F','LF_F_CG','SL_F','SL_F_CG','DSATUR_F','DSATUR_F_CG']
cols_int  = ['Simples','Simples_CG','LF','LF_CG','SL','SL_CG','DSATUR','DSATUR_CG']

palette_frac = {
    'Simples_F':    ('#378ADD', '-'),
    'Simples_F_CG': ('#85B7EB', '--'),
    'LF_F':         ('#1D9E75', '-'),
    'LF_F_CG':      ('#5DCAA5', '--'),
    'SL_F':         ('#D85A30', '-'),
    'SL_F_CG':      ('#F09595', '--'),
    'DSATUR_F':     ('#534AB7', '-'),
    'DSATUR_F_CG':  ('#AFA9EC', '--'),
}

palette_int = {
    'Simples':  ('#378ADD', '-'),
    'LF':       ('#1D9E75', '-'),
    'SL':       ('#D85A30', '-'),
    'DSATUR':   ('#534AB7', '-'),
}

def nome_label(col):
    return col.replace('_F_CG', '_CG').replace('_F', '')

ns = sorted(df['n'].unique())
ds = sorted(df['d'].unique())

# ── 1. Curvas de escala por n (fixando d) ────────────────────────────────────
for d_val in ds:
    subset = df[df['d'] == d_val].sort_values('n')
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    fig.suptitle(f'Escala por n — densidade {d_val}%', fontsize=13)

    base_cols = ['Simples_F', 'LF_F', 'SL_F', 'DSATUR_F']
    cg_cols   = ['Simples_F_CG', 'LF_F_CG', 'SL_F_CG', 'DSATUR_F_CG']

    for col in base_cols:
        color, ls = palette_frac[col]
        axes[0].plot(subset['n'], subset[col], color=color, linestyle=ls,
                     linewidth=1.5, marker='o', markersize=4, label=nome_label(col))
    axes[0].set_title('Sem CG', fontsize=11)
    axes[0].set_xlabel('n'); axes[0].set_ylabel('Média de cores')
    axes[0].legend(fontsize=9); axes[0].grid(alpha=0.3)

    for col in cg_cols:
        color, ls = palette_frac[col]
        axes[1].plot(subset['n'], subset[col], color=color, linestyle=ls,
                     linewidth=1.5, marker='o', markersize=4, label=nome_label(col))
    axes[1].set_title('Com CG', fontsize=11)
    axes[1].set_xlabel('n'); axes[1].set_ylabel('Média de cores')
    axes[1].legend(fontsize=9); axes[1].grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'graficos/escala_d{d_val}.png', dpi=150)
    plt.close()

for d_val in ds:
    subset = df[df['d'] == d_val].sort_values('n')
    fig, ax = plt.subplots(figsize=(9, 5))
    fig.suptitle(f'Ganho do CG por n — densidade {d_val}%', fontsize=13)

    pairs = [('Simples_F','Simples_F_CG','#378ADD'),
             ('LF_F','LF_F_CG','#1D9E75'),
             ('SL_F','SL_F_CG','#D85A30'),
             ('DSATUR_F','DSATUR_F_CG','#534AB7')]

    for base, cg, color in pairs:
        diff = subset[base].values - subset[cg].values
        ax.plot(subset['n'], diff, color=color, linewidth=1.5,
                marker='o', markersize=4, label=nome_label(base))

    ax.axhline(0, color='black', linewidth=0.8, linestyle='--')
    ax.set_xlabel('n'); ax.set_ylabel('Redução de cores (base − CG)')
    ax.legend(fontsize=9); ax.grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'graficos/ganho_cg_d{d_val}.png', dpi=150)
    plt.close()

# ── 2. Sensibilidade à densidade (fixando n) ─────────────────────────────────
for n_val in ns:
    subset = df[df['n'] == n_val].sort_values('d')

    base_cols = ['Simples_F', 'LF_F', 'SL_F', 'DSATUR_F']
    cg_cols   = ['Simples_F_CG', 'LF_F_CG', 'SL_F_CG', 'DSATUR_F_CG']

    # Opção 1: sem CG vs com CG
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    fig.suptitle(f'Sensibilidade à densidade — n={n_val}', fontsize=13)

    for col in base_cols:
        color, ls = palette_frac[col]
        axes[0].plot(subset['d'], subset[col], color=color, linestyle=ls,
                     linewidth=1.5, marker='o', markersize=5, label=nome_label(col))
    axes[0].set_title('Sem CG', fontsize=11)
    axes[0].set_xlabel('Densidade (%)'); axes[0].set_ylabel('Média de cores')
    axes[0].set_xticks(ds); axes[0].legend(fontsize=9); axes[0].grid(alpha=0.3)

    for col in cg_cols:
        color, ls = palette_frac[col]
        axes[1].plot(subset['d'], subset[col], color=color, linestyle=ls,
                     linewidth=1.5, marker='o', markersize=5, label=nome_label(col))
    axes[1].set_title('Com CG', fontsize=11)
    axes[1].set_xlabel('Densidade (%)'); axes[1].set_ylabel('Média de cores')
    axes[1].set_xticks(ds); axes[1].legend(fontsize=9); axes[1].grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'graficos/densidade_n{n_val}.png', dpi=150)
    plt.close()

    # Opção 2: ganho do CG por densidade
    fig, ax = plt.subplots(figsize=(9, 5))
    fig.suptitle(f'Ganho do CG por densidade — n={n_val}', fontsize=13)

    pairs = [('Simples_F','Simples_F_CG','#378ADD'),
             ('LF_F','LF_F_CG','#1D9E75'),
             ('SL_F','SL_F_CG','#D85A30'),
             ('DSATUR_F','DSATUR_F_CG','#534AB7')]

    for base, cg, color in pairs:
        diff = subset[base].values - subset[cg].values
        ax.plot(subset['d'], diff, color=color, linewidth=1.5,
                marker='o', markersize=5, label=nome_label(base))

    ax.axhline(0, color='black', linewidth=0.8, linestyle='--')
    ax.set_xlabel('Densidade (%)'); ax.set_ylabel('Redução de cores (base − CG)')
    ax.set_xticks(ds); ax.legend(fontsize=9); ax.grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'graficos/ganho_cg_n{n_val}.png', dpi=150)
    plt.close()

    print(f'Salvo: densidade_n{n_val}.png e ganho_cg_n{n_val}.png')

# ── 3. Heatmap n × d ─────────────────────────────────────────────────────────
for col in cols_frac + cols_int:
    pivot = df.pivot(index='n', columns='d', values=col)
    fig, ax = plt.subplots(figsize=(7, 6))
    im = ax.imshow(pivot.values, aspect='auto', cmap='Blues',
                   origin='lower', interpolation='nearest')
    ax.set_xticks(range(len(ds))); ax.set_xticklabels([f'{d}%' for d in ds])
    ax.set_yticks(range(len(ns))); ax.set_yticklabels([f'n={n}' for n in ns])
    ax.set_xlabel('Densidade'); ax.set_ylabel('n')
    ax.set_title(f'Heatmap — {col}', fontsize=12)
    for i, n_val in enumerate(ns):
        for j, d_val in enumerate(ds):
            val = pivot.loc[n_val, d_val]
            ax.text(j, i, f'{val:.1f}', ha='center', va='center',
                    fontsize=8, color='white' if val > pivot.values.max()*0.6 else 'black')
    plt.colorbar(im, ax=ax, label='Média de cores')
    plt.tight_layout()
    plt.savefig(f'graficos/heatmap_{col}.png', dpi=150)
    plt.close()
    print(f'Salvo: heatmap_{col}.png')

# ── 4. Impacto do CG ─────────────────────────────────────────────────────────
cg_pairs = [('Simples_F','Simples_F_CG'), ('LF_F','LF_F_CG'),
            ('SL_F','SL_F_CG'), ('DSATUR_F','DSATUR_F_CG')]

fig, axes = plt.subplots(2, 2, figsize=(14, 10))
fig.suptitle('Impacto do CG — melhora % (média sobre densidades)', fontsize=13)
axes = axes.flatten()

for idx, (base, cg) in enumerate(cg_pairs):
    pcts = []
    for n_val in ns:
        rows = df[df['n'] == n_val]
        avg_base = rows[base].mean()
        avg_cg   = rows[cg].mean()
        pct = (avg_base - avg_cg) / avg_base * 100
        pcts.append(pct)
    colors = ['#1D9E75' if p > 0 else '#E24B4A' for p in pcts]
    axes[idx].bar(ns, pcts, color=colors, width=60)
    axes[idx].axhline(0, color='black', linewidth=0.8)
    axes[idx].set_title(f'{base} → {cg}', fontsize=10)
    axes[idx].set_xlabel('n'); axes[idx].set_ylabel('Melhora (%)')
    axes[idx].set_xticks(ns); axes[idx].tick_params(axis='x', rotation=45)
    axes[idx].grid(axis='y', alpha=0.3)

plt.tight_layout()
plt.savefig('graficos/impacto_cg.png', dpi=150)
plt.close()
print('Salvo: impacto_cg.png')

# ── 5. Gap fracionário vs inteiro ────────────────────────────────────────────
gap_pairs = [('Simples','Simples_F','#378ADD'),
             ('LF','LF_F','#1D9E75'),
             ('SL','SL_F','#D85A30'),
             ('DSATUR','DSATUR_F','#534AB7')]

for d_val in ds:
    subset = df[df['d'] == d_val].sort_values('n')
    fig, ax = plt.subplots(figsize=(9, 5))
    for int_col, frac_col, color in gap_pairs:
        gap_pct = (subset[int_col].values - subset[frac_col].values) / subset[int_col].values * 100
        ax.plot(subset['n'], gap_pct, color=color, linewidth=1.5,
                marker='o', markersize=4, label=int_col.replace('_F',''))
    ax.set_title(f'Gap fracionário vs inteiro — densidade {d_val}%', fontsize=12)
    ax.set_xlabel('n'); ax.set_ylabel('Gap (%)')
    ax.legend(fontsize=9); ax.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig(f'graficos/gap_d{d_val}.png', dpi=150)
    plt.close()
    print(f'Salvo: gap_d{d_val}.png')

print('\nPronto! Todos os gráficos estão na pasta graficos/')

# ── Ranking das heurísticas ───────────────────────────────────────────────────

base_cols = ['Simples_F', 'LF_F', 'SL_F', 'DSATUR_F']
cg_cols   = ['Simples_F_CG', 'LF_F_CG', 'SL_F_CG', 'DSATUR_F_CG']

def calcular_ranking(cols):
    medias  = {col: df[col].mean() for col in cols}
    vitorias = {col: 0 for col in cols}

    for _, row in df.iterrows():
        melhor = min(row[col] for col in cols)
        for col in cols:
            if row[col] == melhor:
                vitorias[col] += 1

    ranking = sorted(cols, key=lambda c: medias[c])
    return ranking, medias, vitorias

fig, axes = plt.subplots(1, 2, figsize=(14, 5))
fig.suptitle('Ranking das heurísticas fracionárias', fontsize=13)

for ax, cols, titulo in [(axes[0], base_cols, 'Sem CG'), (axes[1], cg_cols, 'Com CG')]:
    ranking, medias, vitorias = calcular_ranking(cols)

    nomes   = [c.replace('_F_CG','_CG').replace('_F','') for c in ranking]
    valores = [medias[c] for c in ranking]
    wins    = [vitorias[c] for c in ranking]
    cores   = ['#534AB7', '#1D9E75', '#D85A30', '#378ADD']

    bars = ax.barh(nomes, valores, color=cores[:len(ranking)], height=0.5)

    for bar, win, val in zip(bars, wins, valores):
        ax.text(val + 0.3, bar.get_y() + bar.get_height()/2,
        f'média {val:.2f} | {win} vitórias',
        va='center', fontsize=9, color='#888780')

    ax.set_title(titulo, fontsize=11)
    ax.set_xlabel('Média de cores (menor = melhor)')
    ax.invert_yaxis()
    ax.grid(axis='x', alpha=0.3)
    ax.set_xlim(0, max(valores) * 1.35)

plt.tight_layout()
plt.savefig('graficos/ranking_heuristicas.png', dpi=150)
plt.close()
print('Salvo: ranking_heuristicas.png')

# Imprime também no terminal
for cols, titulo in [(base_cols, 'Sem CG'), (cg_cols, 'Com CG')]:
    ranking, medias, vitorias = calcular_ranking(cols)
    print(f'\nRanking — {titulo}')
    print(f'{"#":<4} {"Heurística":<20} {"Média":>8} {"Vitórias":>10}')
    print('-' * 46)
    for i, col in enumerate(ranking, 1):
        print(f'{i:<4} {col:<20} {medias[col]:>8.2f} {vitorias[col]:>10}')