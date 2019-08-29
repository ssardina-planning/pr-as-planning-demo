from pynput.keyboard import Key, Listener
from pynput import keyboard
import json
import ai2thor.controller
import inquirer
from shapely.geometry import Point, Polygon

controller = ai2thor.controller.Controller()
controller.local_executable_path = "/home/yiding/Documents/pr-as-planning-demo/thor/thor-linux/thor-local-Linux64"
event = controller.start(player_screen_width=1000, player_screen_height=666)
controller.step(dict(action='Initialize', gridSize=0.25, visibilityDistance=1))

rotation = 245

hand_empty = True
object_on_hand = {}


foyer_coords = [(2.041, 11.579), (2.041, 8.4475), (-4.2456, 8.4475), (-4.2456, 11.576)]
bedroom_coords = [(-4.3336, 11.105), (-4.3336, 7.305), (-9.7336, 7.305), (-9.7336, 11.105)]
livingroom_coords= [(6.0463, 8.4976), (6.0463, 2.27), (-2.3615, 2.27), (-2.3615, 8.4976)]
bathroom_coords=[(6.568, 1.198), (6.568, -0.055355), (4.9768, -0.055355), (4.9768, -2.002),(2.368,-2.002),(2.368,1.198)]
kitchen_coords=[(2.43, 2.5), (2.43, -2.9), (-2.4, -2.9), (-2.4, 2.5)]
shower_coords=[(6.568,-0.055355),(6.568,-2.002),(4.9768,-2.002),(4.9768,-0.055355)]
foyer = Polygon(foyer_coords)
bedroom=Polygon(bedroom_coords)
livingroom=Polygon(livingroom_coords)
bathroom=Polygon(bathroom_coords)
kitchen=Polygon(kitchen_coords)
shower=Polygon(shower_coords)

current_location="Foyer";

f = open("observations.log", "w", encoding="utf-8")

# def on_press(key):
#     global rotation
#     global event
#     try:
#         if key.char == "w":
#             event = controller.step(dict(action='MoveAhead'))

#         if key.char == "s":
#             event = controller.step(dict(action='MoveBack'))

#         if key.char == "a":
#             event = controller.step(dict(action='MoveLeft'))
#         if key.char == "d":
#             event = controller.step(dict(action='MoveRight'))

#         if key.char == "e":
#             rotation += 5
#             event = controller.step(dict(action='Rotate', rotation=rotation))
#         if key.char == "q":
#             rotation -= 5
#             event = controller.step(dict(action='Rotate', rotation=rotation))
#         check_location()
#     except AttributeError:
#         pass

def check_location():
    global event
    global current_location
    newpoint=Point(event.metadata['agent']['position']['x'],event.metadata['agent']['position']['z'])
    new_location=""
    if foyer.contains(newpoint):
        new_location="Foyer"
    elif bedroom.contains(newpoint):
        new_location="Bedroom"
    elif livingroom.contains(newpoint):
        new_location="Living_Room"
    elif kitchen.contains(newpoint):
        new_location="Kitchen"
    elif bathroom.contains(newpoint):
        new_location="Bathroom"
    elif shower.contains(newpoint):
        new_location = "Shower"
    else:
        new_location="unknown"
    if new_location!="unknown" and new_location != current_location:
        writefile("(WalkTo {0} {1})".format(current_location,new_location))
        current_location=new_location

def writefile(message):
    print(message)
    f.write(message+"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")
    f.flush()
    f.seek(0,0)


def handle_open():
    global event
    global current_location
    objects_map = {}
    for o in event.metadata['objects']:
        if o['visible'] and o['openable']:
            objects_map[o['objectId']] = o

    if not objects_map:
        return

    objectId=ask_questions("Which Object you want to open/close?",objects_map)
    open_status = objects_map[objectId]['isopen']

    if open_status:
        event = controller.step(dict(action='CloseObject', objectId=objectId))

        if event.metadata["lastActionSuccess"] == False:
            print("Close Failed: "+event.metadata["errorMessage"])
        else:
            writefile("(Close {0} {1})".format(current_location,objects_map[objectId]['name']))


    else:
        event = controller.step(dict(action='OpenObject', objectId=objectId))
        if event.metadata["lastActionSuccess"] == False:
            print("Open Failed: "+event.metadata["errorMessage"])

        else:
            writefile("(Open {0} {1})".format(current_location, objects_map[objectId]['name']))


def handle_pick():
    global hand_empty
    global event
    global object_on_hand

    if len(event.metadata["inventoryObjects"]) == 0:
        hand_empty = True
    else:
        hand_empty = False

    if hand_empty:
        objectMap=get_pickable_objects_map(event)
        if not objectMap:
            return
        objectId=ask_questions("Which Object to Pick Up",objectMap)
        event = controller.step(dict(action='PickupObject', objectId=objectId))
        object_on_hand = objectMap[objectId]
        if not event.metadata["lastActionSuccess"]:
            print("Error Happen: "+event.metadata["errorMessage"])
        else:
            # print("Picked Up: " + objectId)
            writefile("(PickUp {0} {1})".format(objectMap[objectId]['name'],current_location))

    else:
        objectMap=get_receptacle_objects_map(event)
        if not objectMap:
            return
        receptacleObjectId=ask_questions("Where to put object?",objectMap)
        event = controller.step(
            dict(action='PutObject', objectId=object_on_hand["objectId"], receptacleObjectId=receptacleObjectId))

        if not event.metadata["lastActionSuccess"]:
            print(event.metadata["errorMessage"])
        else:
            # print("Drop Object: " + object_on_hand["objectId"] + " on " + receptacleObjectId)
            writefile("(Drop {0} {1})".format(object_on_hand['name'],current_location))

def handle_toggle():
    global event
    objects_map = get_toggle_objects_map(event)

    if not objects_map:
        return

    objectId=ask_questions("Which Object you want to On/Off?",objects_map)
    toggle_status = objects_map[objectId]['istoggled']
    if toggle_status:
        event = controller.step(dict(action='ToggleObjectOff', objectId=objectId))

        if event.metadata["lastActionSuccess"] == False:
            print("Turn Off Failed: "+event.metadata["errorMessage"])
        else:
            writefile("(TurnOff {0})".format(objects_map[objectId]['name']))


    else:
        event = controller.step(dict(action='ToggleObjectOn', objectId=objectId))
        if event.metadata["lastActionSuccess"] == False:
            print("Toggle On Failed: "+event.metadata["errorMessage"])

        else:
            writefile("(TurnOn {0})".format(objects_map[objectId]['name']))

def ask_questions(message,objectMap):

    questions = [
        inquirer.List('objectId',
                      message=message,
                      choices=objectMap.keys(),
                      ),
    ]
    objectId=inquirer.prompt(questions)['objectId']
    return objectId

def get_toggle_objects_map(event):
    result = {}
    for o in event.metadata['objects']:
        if o['visible'] and o['toggleable']:
            result[o['objectId']] = o
    return result

def get_pickable_objects_map(event):
    result={}
    for o in event.metadata['objects']:
        if o['visible'] and o['pickupable']:
            result[o['objectId']]=o
    return result

def get_receptacle_objects_map(event):
    result = {}
    for o in event.metadata['objects']:
        if (o['visible'] and o['receptacle'] and o['openable'] == False) or (
                            o['visible'] and o['receptacle'] and o['openable'] == True and o['isopen'] == True):
            result[o['objectId']]=o
    return result

def on_release(key):
    if key == Key.esc:
        # Stop listener
        return False
    global rotation
    global event
    try:
        if key == Key.up:
            event = controller.step(dict(action='LookUp'))
        if key == Key.down:
            event = controller.step(dict(action='LookDown'))
        if key == Key.left:
            rotation -= 10
            event = controller.step(dict(action='Rotate', rotation=rotation))
        if key == Key.right:
            rotation += 10
            event = controller.step(dict(action='Rotate', rotation=rotation))
        if key.char == "p":
            handle_pick()
        if key.char == "o":
            handle_open()
        if key.char == "t":
            handle_toggle()
            #Thor has issue with toggle light
            event = controller.step(dict(action='Rotate', rotation=rotation))
        if key.char == "m":
            event = controller.step(dict(action='MoveHandAhead', moveMagnitude=0.1))
        if key.char == "w":
            event = controller.step(dict(action='MoveAhead'))
        if key.char == "s":
            event = controller.step(dict(action='MoveBack'))
        if key.char == "a":
            event = controller.step(dict(action='MoveLeft'))
        if key.char == "d":
            event = controller.step(dict(action='MoveRight'))
        
        check_location()
    except AttributeError:
        pass

# Collect events until released
with Listener(
        # on_press=on_press,
        on_release=on_release) as listener:
    listener.join()
