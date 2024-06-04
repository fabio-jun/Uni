import tkinter as tk
from tkinter import ttk

CORES = {
    "PRETO": 0,
    "MARROM": 1,
    "VERMELHO": 2,
    "LARANJA": 3,
    "AMARELO": 4,
    "VERDE": 5,
    "AZUL": 6,
    "VIOLETA": 7,
    "CINZA": 8,
    "BRANCO": 9,
}

MULTIPLICADORES = {
    "PRETO": 1,
    "MARROM": 10,
    "VERMELHO": 100,
    "LARANJA": 1000,
    "AMARELO": 10000,
    "VERDE": 100000,
    "AZUL": 1000000,
    "VIOLETA": 10000000,
    "DOURADO": 0.1,
    "PRATEADO": 0.01,
}

TOLERANCIAS = {
    "MARROM": 1,
    "VERMELHO": 2,
    "VERDE": 0.5,
    "AZUL": 0.25,
    "VIOLETA": 0.1,
    "CINZA": 0.05,
    "DOURADO": 5,
    "PRATEADO": 10,
}

def obter_cor(var, cores):
    return cores.get(var.get(), 0)

def obter_tolerancia(var):
    return TOLERANCIAS.get(var.get(), 0)

def obter_multiplicador(var, multiplicadores):
    return multiplicadores.get(var.get(), 0)

def calcular_resistencia_e_tolerancia_4():
    faixa1 = obter_cor(faixa_vars_4[0], CORES)
    faixa2 = obter_cor(faixa_vars_4[1], CORES)
    faixa3 = obter_multiplicador(faixa_vars_4[2], MULTIPLICADORES)
    faixa4 = obter_tolerancia(faixa_vars_4[3])

    resistencia = (faixa1 * 10 + faixa2) * faixa3
    resultado_label.config(text=f"Resistência: {resistencia} ohms\nTolerância: +- {faixa4:.2f}%")

def calcular_resistencia_e_tolerancia_5():
    faixa1 = obter_cor(faixa_vars_5[0], CORES)
    faixa2 = obter_cor(faixa_vars_5[1], CORES)
    faixa3 = obter_cor(faixa_vars_5[2], CORES)
    faixa4 = obter_multiplicador(faixa_vars_5[3], MULTIPLICADORES)
    faixa5 = obter_tolerancia(faixa_vars_5[4])

    resistencia = (faixa1 * 100 + faixa2 * 10 + faixa3) * faixa4
    resultado_label.config(text=f"Resistência: {resistencia} ohms\nTolerância: +- {faixa5:.2f}%")

def atualizar_widgets(*args):
    numero_faixas = int(faixas_combobox.get())

    # Remover todos os widgets existentes
    for i in range(5):
        if faixa_labels[i]:
            faixa_labels[i].destroy()
        if faixa_comboboxes[i]:
            faixa_comboboxes[i].destroy()

    # Criar novos widgets
    if numero_faixas == 4:
        for i in range(numero_faixas):
            label = ttk.Label(frame, text=f"Faixa {i + 1}:")
            label.grid(column=0, row=i + 1, padx=5, pady=5, sticky=tk.W)
            faixa_labels[i] = label

            if i == 2:  # Terceira faixa (faixa 3) em resistores de 4 faixas
                combobox = ttk.Combobox(frame, values=list(MULTIPLICADORES.keys()), state="readonly", textvariable=faixa_vars_4[i])
            elif i == 3:  # Quarta faixa (faixa 4) em resistores de 4 faixas
                combobox = ttk.Combobox(frame, values=list(TOLERANCIAS.keys()), state="readonly", textvariable=faixa_vars_4[i])
            else:
                combobox = ttk.Combobox(frame, values=list(CORES.keys()), state="readonly", textvariable=faixa_vars_4[i])

            combobox.grid(column=1, row=i + 1, padx=5, pady=5, sticky=tk.W)
            faixa_comboboxes[i] = combobox
        calcular_button.config(command=calcular_resistencia_e_tolerancia_4)  # Atualiza a função do botão
    elif numero_faixas == 5:
        for i in range(numero_faixas):
            label = ttk.Label(frame, text=f"Faixa {i + 1}:")
            label.grid(column=0, row=i + 1, padx=5, pady=5, sticky=tk.W)
            faixa_labels[i] = label

            if i == 3:  # Quarta faixa (faixa 4) em resistores de 5 faixas
                combobox = ttk.Combobox(frame, values=list(MULTIPLICADORES.keys()), state="readonly", textvariable=faixa_vars_5[i])
            elif i == 4:  # Quinta faixa (faixa 5) em resistores de 5 faixas
                combobox = ttk.Combobox(frame, values=list(TOLERANCIAS.keys()), state="readonly", textvariable=faixa_vars_5[i])
            else:
                combobox = ttk.Combobox(frame, values=list(CORES.keys()), state="readonly", textvariable=faixa_vars_5[i])

            combobox.grid(column=1, row=i + 1, padx=5, pady=5, sticky=tk.W)
            faixa_comboboxes[i] = combobox
        calcular_button.config(command=calcular_resistencia_e_tolerancia_5)  # Atualiza a função do botão

# Configuração da interface gráfica
root = tk.Tk()
root.title("Calculadora de Resistores - IEEE")

# Ajuste do tamanho da janela
largura_janela = 400
altura_janela = 300
root.geometry(f"{largura_janela}x{altura_janela}")

# Variáveis de controle para as opções do usuário
faixa_vars_4 = [tk.StringVar() for _ in range(4)]
faixa_vars_5 = [tk.StringVar() for _ in range(5)]

# Configuração do layout
frame = ttk.Frame(root, padding="10")
frame.grid(column=0, row=0, sticky=(tk.W, tk.E, tk.N, tk.S))

# Número de faixas
faixas_label = ttk.Label(frame, text="Número de faixas:")
faixas_label.grid(column=0, row=0, padx=5, pady=5, sticky=tk.W)

faixas_combobox = ttk.Combobox(frame, values=["4", "5"], state="readonly")
faixas_combobox.grid(column=1, row=0, padx=5, pady=5, sticky=tk.W)
faixas_combobox.bind("<<ComboboxSelected>>", atualizar_widgets)

# Cores das faixas e tolerância
faixa_labels = [None] * 5
faixa_comboboxes = [None] * 5

for i in range(5):
    label = ttk.Label(frame, text=f"Faixa {i + 1}:")
    label.grid(column=0, row=i + 1, padx=5, pady=5, sticky=tk.W)
    faixa_labels[i] = label

    combobox = ttk.Combobox(frame, values=list(CORES.keys()), state="readonly", textvariable=faixa_vars_5[i])
    combobox.grid(column=1, row=i + 1, padx=5, pady=5, sticky=tk.W)
    faixa_comboboxes[i] = combobox

# Resultado
resultado_label = ttk.Label(frame, text="")
resultado_label.grid(column=0, row=6, columnspan=2, pady=10)

# Botão de cálculo
calcular_button = ttk.Button(frame, text="Calcular", command=calcular_resistencia_e_tolerancia_5)
calcular_button.grid(column=0, row=7, columnspan=2, pady=10)

# Inicie a interface gráfica
root.mainloop()
