# Retro watch user guide



## Home screen

The home screen is divided into 3 vertical sections. The upper part always stays visible, just like the notification bar in smartphones.

![webhooks]( 1.png )

The middle section displays actual time

![webhooks]( 2.png )

The lower part displays misc data such as temperature and air pressure

![webhooks]( 3.png )

## Navigation

There are only three buttons. 

Foto z zaznaczonymi strzałkami opisami przycisków

To select an option press the "Up" key for a little longer, we will refer to this as "OK". To go back to the previous screen or cancel, press "Down" for a little longer. This gesture is going to be refered to as "Back"

To make things more obvious - "Back" works just like the "back" button in the browser, while "Ok" works just like "Enter" on regular PCs.

The watch has two main views - you can change them by pressing either "Up" or "Down" from any of these views. 

On a `Home screen`, there are no interactive components.
![webhooks]( 4.png )
On the second screen

![webhooks]( 5.png )
by pressing "Ok" (long press "Up"), you will select the current screen as an active one. This is indicated by a long horizontal line at the bottom of the screen

![webhooks]( 6.png )

To go back to the home screen, press "Back" (long press "Down") and "Down".

While having the menu selected, you can browse through the list of available apps. Use "Up" and "Down" buttons.

## Menu

### Weather
Given internet access, weather app can download the weather for your location.
When entering the app for the first time you will be asked to type in the name of your city. 
### Stopwatch

### Games

### IFTTT

IFTTT is an application that connects multiple services together.
With the help of IFTTT it is possible to automate various tasks from the watch, for instance smart home light, sending mails, opening the garage gate and much more.

An IFTTT event may trigger on any action that you specify. In our case it is going to be a signal coming from the watch you can activate by hand. The list of available services is shown at https://ifttt.com/services

1. Create an account on https://ifttt.com/join

2. Go to - https://ifttt.com/create/

3. Click `+This`, a new menu shall open. Type `webhooks` in the search bar. Click a tile 

![webhooks]( webhooks.png )

Next, click 

![webhooks]( receive.png )

4. The following window should open

![webhooks]( name.png )

Type in any name that comes to your mind, such as `garage_door` (do not use spaces though!). Remember that name.

5. Click `Create trigger`

6. Click `+That`

7. A new window will open. Here, you can select from a number of available services. In most cases, you would have to connect your IFTTT account to an account on external service. For instance: you can order pizza, open a phone app or send a HTTP request.

8. For demonstration, I made the simplest thing possible - a web request. 

![webhooks]( final.png )


You can access the list of active events at https://ifttt.com/my_applets

You can read more about webhooks at: https://help.ifttt.com/hc/en-us/articles/115010230347-Webhooks-service-FAQ

#### Linking to the watch

#### IFTTT

Go to https://ifttt.com/maker_webhooks/ and click `Documentation`

![webhooks]( docsbutton.png )

A new page will open. 

![webhooks]( docs.png )

You need to copy the key or write it down on a paper so you can provide it later.

#### Watch

Enter the watch config, click `configure` next to IFTTT in the section `apps`

Paste or type in the key from previous step in the text field next to the `key` field.

To add new actions, click `add new key`, the field to the left is a human - friendly name, such as `Garage door`. Next, fill the field to the right with the event name.

If you forget the name, go to https://ifttt.com/my_applets

![webhooks]( eventtile.png ) `garage_door` is the name you should provide.


### Status

### WiFi

### Settings


## Web config