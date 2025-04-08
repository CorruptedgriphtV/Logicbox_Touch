import tkinter as tk
from tkinter import messagebox

def respuesta_si():
    messagebox.showinfo("Respuesta", "¡Gracias! Eres increíble.")

def respuesta_no():
    messagebox.showinfo("Respuesta", "Oh, lo siento. esa respuesta .")

if __name__ == "__main__":
    root = tk.Tk()
    root.title("Pregunta Importante")

    # Etiqueta de la pregunta
    label_pregunta = tk.Label(root, text="¿Quieres ser mi novia?")
    label_pregunta.pack(padx=20, pady=20)

    # Marco para los botones (ponen los botones de forma estable)
    frame_botones = tk.Frame(root)
    frame_botones.pack(padx=10, pady=10)

    btn_si = tk.Button(frame_botones, text="Sí", command=respuesta_si, width=10)
    btn_si.grid(row=0, column=0, padx=10)

    btn_no = tk.Button(frame_botones, text="No", command=respuesta_no, width=10)
    btn_no.grid(row=0, column=1, padx=10)

    root.mainloop()
