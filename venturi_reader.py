##################################################
# Python + Arduino Datalogger
##################################################
#
# This code reads from the serial port and keeps
# the data in memory until the user presses
# CTRL+C, wherein the data is then saved locally to
# a .csv file with the current timestamp. Lastly,
# the data is read from the file and plotted
#
##################################################
#
import numpy as np
import matplotlib.pyplot as plt
import serial,datetime,csv,os
import serial.tools.list_ports as COMs
#
#
############################################
# Find Arudino ports, select one,
# start communication with it
############################################
#
arduino_ports = [ii.device for ii in COMs.comports() if\
                 len((ii.device).split('ttyACM'))>1 or\
                 len((ii.device.split('ttyUSB')))>1]
ser = serial.Serial(arduino_ports[0],baudrate=9600) # match baud on Arduino
ser.flush() # clear the port
#
############################################
# Grabbing real-time data from Arduino
############################################
#
t_now = datetime.datetime.strftime(datetime.datetime.now(),
                                   '%Y_%m_%d_%H_%M_%S')
datafile = 'arduino_data_'+'{0}'.format(t_now)+'.csv' # date
data_array = []; start_bool = False
while True:
    try:
        ser_bytes = ser.readline() # read Arduino serial data
        decoded_bytes = ser_bytes.decode('utf-8') # decode data to utf-8
        data = (decoded_bytes.replace('\r','')).replace('\n','')
        if start_bool==False and data=='Acquisition Start':
            # read the first line after acquisition start as header
            header = ((ser.readline()).decode('utf-8').replace('\r','')).\
                     replace('\n','')
            start_bool = True
            print('Data Acquisition Starting...')
            ser.flush()
            continue
        if start_bool:
            # saving data to variable to incrase speed
            # (saving to file happens after keyboard interrupt)
            data_array.append([float(ii) for ii in data.split(',')])
    except KeyboardInterrupt:
        print('Exiting Loop and Saving Data')
        if data_array == []:
            print('No Valid Data') # if no data, print out and exit
            break
        # save valid data in array to .csv file
        data_array = np.array(data_array) # convert to numpy array
        with open(datafile,'w') as csvfile:
            csv_writer = csv.writer(csvfile,delimiter=',')
            csv_writer.writerow([header])
            for row in data_array:
                csv_writer.writerow(row)
        break # finally, exit loop after save
#
############################################
# Read back the data file and plot the data
############################################
#
if os.path.isfile(datafile):
    data_check = []
    with open(datafile,'r') as csvfile:
        csvreader = csv.reader(csvfile) # read the saved file
        header = next(csvreader)[0].split(',')
        for row in csvreader:
            data_check.append([float(ii) for ii in row])

    data_check = np.array(data_check) # convert to numpy array

    # Plot the data from the saved file to verify
    plt.style.use('ggplot')
    fig,axs = plt.subplots(2,1,figsize=(12,9))
    ax1 = axs[0]
    ax1.scatter(data_check[:,0],data_check[:,1],label=header[1])
    ax1.legend()
    ax1.set_xlabel(header[0]) # use the header for x-label
    ax1.set_ylabel(header[1]) # use the header for y-label
    ax2 = axs[1]
    ax2.scatter(data_check[:,0],data_check[:,2],label=header[2])
    ax2.legend()
    ax2.set_xlabel(header[0]) # use the header for x-label
    ax2.set_ylabel(header[2]) # use the header for y-label
    plt.show() # show the plot
