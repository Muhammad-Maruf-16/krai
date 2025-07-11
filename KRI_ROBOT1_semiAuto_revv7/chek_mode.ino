//cek setting mode  
void checkSettingMode() {
  if (L3 && !alreadyPressedL3) {
    alreadyPressedL3 = true;
    settingMode = true;
    deRPM = beRPM = kaRPM = kiRPM = 0;
  }else if(!L3){
    alreadyPressedL3 = false;
  }
}