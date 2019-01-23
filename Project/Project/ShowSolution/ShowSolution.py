import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as lines
import matplotlib.transforms as mtransforms
import matplotlib.text as mtext

class MyLine(lines.Line2D):
    def __init__(self, *args, **kwargs):
        lines.Line2D.__init__(self, *args, **kwargs)

    def set_figure(self, figure):
        lines.Line2D.set_figure(self, figure)

    def set_axes(self, axes):
        lines.Line2D.set_axes(self, axes)

    def set_transform(self, transform):
        lines.Line2D.set_transform(self, transform)

    def set_data(self, x, y):
        lines.Line2D.set_data(self, x, y)

    def draw(self, renderer):
        lines.Line2D.draw(self, renderer)

class Point():
    def __init__(self, x, y):
        self.x = x
        self.y = y

class Solution():
    def __init__(self, filename):
        self.filename = filename
        self.value = 0
        self.coord = dict()
        self.trucks = dict()
        self.clientstype = dict()
        self.truckstype = dict()
        self.competence = False
        self.load_file()
        if(self.competence):
            self.show_file_competence()
        else:
            self.show_file()

    def load_file(self):
        f = open(self.filename, 'r')
        read_coord = True
        line = f.readline().split()
        if(line[0] == "BASIC"):
            line = f.readline().split()
            if(line[0] == "COORD"):
                for l in f:
                    line = l.split()
                    if(line[0] == "TRUCKS"):
                        break
                    self.coord[int(line[0])] = Point(int(line[1]), int(line[2]))
            for l in f:
                line = l.split()
                if(line[0] == "VALUE"):
                    break
                self.trucks[int(line[0])] = list()
                for i in range(1, len(line)):
                    if(line[i] != "\n"):
                        self.trucks[int(line[0])].append(int(line[i]))
            line = f.readline().split()
            self.value = int(line[0])
            f.close()
            return
        if(line[0] == "COMPETENCE"):
            self.competence = True
            line = f.readline().split()
            if(line[0] == "CLIENTSTYPE"):
                for l in f:
                    line = l.split()
                    # print(line)
                    if(line[0] == "TRUCKSTYPE"):
                        break
                    self.clientstype[int(line[0])] = line[1]
            for l in f:
                line = l.split()
                if(line[0] == "AB"):
                    for i in range(1, len(line)):
                        if(line[i] != "\n"):
                            self.truckstype[int(line[i])] = "AB"
                if(line[0] == "A"):
                    for i in range(1, len(line)):
                        if(line[i] != "\n"):
                            self.truckstype[int(line[i])] = "A"
                if(line[0] == "B"):
                    for i in range(1, len(line)):
                        if(line[i] != "\n"):
                            self.truckstype[int(line[i])] = "B"
                if(line[0] == "EMPTY"):
                    for i in range(1, len(line)):
                        if(line[i] != "\n"):
                            self.truckstype[int(line[i])] = "empty"
                if(line[0] == "COORD"):
                    break
            for l in f:
                line = l.split()
                if(line[0] == "TRUCKS"):
                    break
                self.coord[int(line[0])] = Point(int(line[1]), int(line[2]))
            for l in f:
                line = l.split()
                if(line[0] == "VALUE"):
                    break
                self.trucks[int(line[0])] = list()
                for i in range(1, len(line)):
                    if(line[i] != "\n"):
                        self.trucks[int(line[0])].append(int(line[i]))
            line = f.readline().split()
            self.value = float(line[0])
            f.close()
            return

    def show_file(self):
        colors = ['blue', 'green', 'orange', 'red', 'purple', 'black', 'yellow', 'cyan', 'magenta', 'grey']
        fig, ax = plt.subplots()
        color = 0
        for truck in self.trucks.keys():
            x = list()
            y = list()
            x.append(self.coord[1].x)
            y.append(self.coord[1].y)
            clients = self.trucks[truck]
            for c in clients:
                x.append(self.coord[c].x)
                y.append(self.coord[c].y)

            x.append(self.coord[1].x)
            y.append(self.coord[1].y)
            line = MyLine(x, y, mfc='red', ms=12)
            line.set_color(colors[color])
            ax.add_line(line)
            color = (color + 1) % 10
        ax.set_xlim(left=None, right=100)
        ax.set_ylim(bottom=None, top=100)
        plt.title("Value : "+str(self.value))
        plt.show()

    def show_file_competence(self):
        colors = {"AB" : 'blue', "A" : "green", "B": "red", "empty" : "orange"}
        ls = {"AB" : 0, "A" : 0, "B": 0, "empty" : 0}
        linestyle = ['solid', 'dashed', 'dashdot', 'dotted']
        legend = list()
        fig, ax = plt.subplots()
        for truck in self.trucks.keys():
            x = list()
            y = list()
            x.append(self.coord[1].x)
            y.append(self.coord[1].y)
            clients = self.trucks[truck]
            for c in clients:
                x.append(self.coord[c].x)
                y.append(self.coord[c].y)
            x.append(self.coord[1].x)
            y.append(self.coord[1].y)
            line = MyLine(x, y, mfc='red', ms=12)
            line.set_color(colors[self.truckstype[truck]])
            line.set_linestyle(linestyle[ls[self.truckstype[truck]]])
            ls[self.truckstype[truck]] += 1
            ax.add_line(line)
        print(self.coord.keys())
        print(self.clientstype)
        for client in self.coord.keys():
            # print(self.clientstype[client])
            if(self.clientstype[client] in legend):
                ax.plot(self.coord[client].x, self.coord[client].y, color=colors[self.clientstype[client]] ,marker='o')
                continue
            ax.plot(self.coord[client].x, self.coord[client].y, color=colors[self.clientstype[client]] ,marker='o', label=self.clientstype[client])
            legend.append(self.clientstype[client])
        ax.set_xlim(left=None, right=100)
        ax.set_ylim(bottom=None, top=100)
        ax.legend()
        plt.title("Value : "+str(self.value))
        plt.show()

def main():
    s = Solution("../test.txt")

main()
