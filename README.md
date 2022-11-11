# Portfollio

<p>
  Code only provided for portfollio and not full projects.
</p>

<h2>VR Hip-Tracker Prototype</h2></font>
<p>
  Device to deterimne which direction user is facing so VR movement can be based on hip direction rather than headset direction.
  This allows more realistic movement in VR space. Device connects to app using serial communication.
</p>
<ul>
<li>Arduino mini used for microcontroller.</li>
<li>MPU-6050 sensor used to calculate orientation of device. Also used to calculate X,Y and Z velocity and acceleration of users body</li>
<li>HC-06 for bluetooth support.</li>
<li>Microcontroller programed in C using Arduino Studio IDE.</li>
</ul>
<img align ="left" alt="gif" src="https://github.com/NicholasGennadyKorta/Portfollio/blob/main/README_FILES/VR%20hip-tracker%20Ptototype%20hardware.gif" width=300 height=300>
<img align ="left" alt="gif" src="https://github.com/NicholasGennadyKorta/Portfollio/blob/main/README_FILES/VR%20hip-tracker%20Ptototype%20Unity.gif" width=300 height=300>
<br><br><br><br><br><br><br><br><br><br><br><br><br><br>

<h2>Unreal ECS/ DOTS</h2>
<ul>
<li>Uses one array per component method.</li>
<li>Prioritizes performance but can be wasteful in memory.</li>
<li>Uses macros to create and get components.</li>
<li>Base coded in C++.</li>
<li>Unreal wrapper using Unreal API.</li>
<li>Allow creation of entities in blueprints and ability to modify its data.</li>
<li>Actors can be associated with one or more entities.</li>  
</ul>
<h4>Movement stress test</h4>
<img align ="left" alt="gif" src="https://github.com/NicholasGennadyKorta/Portfollio/blob/main/README_FILES/Unreal%20ECS%20Movement%20Test.gif" width=300 height=300>
<br><br><br><br><br><br><br><br><br><br><br><br><br><br>
<h4>ECS / DOTS Character moudle</h4>
<p>
Character input, stride/oritation movement components. More optimized than blendspaces and simpler for animators as well. Best for stylized games where motion does not need to look realistic.
3 modes. 
</p>
<ul>
<li>1 animation (good for robot walkers that can twist whole body around).</li>
<li>2 animation (forward / back).</li>
<li>4 animation (forward / back / left / right).</li>
</ul>
</p>

https://user-images.githubusercontent.com/8712701/201415326-6129ec2c-a738-4337-90c3-21c73e5f0d30.mp4

<h2>Narrator</h2>

<h4>Scripting language / Editor</h4>

https://user-images.githubusercontent.com/8712701/201438578-e5bc0f24-ad16-4e69-92fc-a0f8747cbf1f.mp4

<h4>Localization</h4>
<p>
Can auto translate using Google Cloud API.
Explort/Import TSV / JSON files for outsourced translations.
</p>

https://user-images.githubusercontent.com/8712701/201443326-429256ac-f071-44e1-8f17-fdc28b1b3c29.mp4


<h2>Dungeon Generator</h2>

<h4>Unreal : Isscac profile (Based on binding of issac)</h4>

https://user-images.githubusercontent.com/8712701/201423436-f66f6f53-f14f-4241-ae72-487d3c97d0a3.mp4

<h4>Unity : Isscac profile (Based on binding of issac)</h4>

https://user-images.githubusercontent.com/8712701/201434391-1fb4f523-8c65-4c28-9fd1-815cf0e537c9.mp4

<h4>Unity : Walker profile (Based on binding of issac)</h4>

https://user-images.githubusercontent.com/8712701/201435581-de374876-f92f-4b34-9bcd-11f249eeac6b.mp4


