import tkinter as tk
from tkinter import messagebox
import webbrowser
import csv

# Function to read bin data from the CSV file
def read_bin_data_from_csv(file_path):
    bins = []
    try:
        with open(file_path, mode='r') as file:
            csv_reader = csv.DictReader(file)
            for row in csv_reader:
                bins.append({
                    "name": row["name"],
                    "lat": float(row["lat"]),
                    "long": float(row["long"]),
                    "fill_level": int(row["fill_level"])
                })
    except FileNotFoundError:
        messagebox.showerror("Error", "CSV file not found!")
    return bins

# Optimization Logic: Filter areas with bin fill levels above the threshold
def optimize_route():
    threshold = 70
    bins = read_bin_data_from_csv("D:\Documents\DESIGN SPRINTS\waste_bins.csv")  # Updated file path
    optimized_areas = [area for area in bins if area['fill_level'] >= threshold]
    
    if optimized_areas:
        route_names = ', '.join([area['name'] for area in optimized_areas])
        messagebox.showinfo("Optimized Route", f"Visit: {route_names}")
        open_google_maps(optimized_areas)
    else:
        messagebox.showinfo("Optimized Route", "No bins need collection right now.")

# Function to open Google Maps with the optimized route
def open_google_maps(optimized_areas):
    if not optimized_areas:
        return
    
    start = optimized_areas[0]  # Starting point (first area)
    waypoints = optimized_areas[1:]  # Remaining areas as waypoints

    # Build the URL for Google Maps
    maps_url = "https://www.google.com/maps/dir/?api=1"
    origin = f"{start['lat']},{start['long']}"
    destination = f"{waypoints[-1]['lat']},{waypoints[-1]['long']}" if waypoints else origin
    waypoint_str = "|".join([f"{area['lat']},{area['long']}" for area in waypoints])

    full_url = f"{maps_url}&origin={origin}&destination={destination}&waypoints={waypoint_str}"
    
    # Open the route in the browser
    webbrowser.open(full_url)

# GUI for the Route Optimization App
root = tk.Tk()
root.title("Waste Collection Route Optimizer")

# GUI Label
label = tk.Label(root, text="Click to Optimize the Route")
label.pack(pady=10)

# Optimize Route Button
optimize_button = tk.Button(root, text="Optimize Route", command=optimize_route)
optimize_button.pack(pady=20)

# Set window size and start the Tkinter main loop
root.geometry("300x150")
root.mainloop()
