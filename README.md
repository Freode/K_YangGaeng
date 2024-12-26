# K_YangGaeng
Unreal Engine 5 기반으로 제작한 멀티플레이 TPS 게임 제작 프로젝트


<img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaneg_Title_Image.png?raw=true" alt="Image 9" width="500">


## 요약
Unreal Engine으로 게임 제작을 희망하는 학술동아리 학우들과 함께 진행한 게임 제작 프로젝트입니다.
크래프톤 사의 Play Unknown's BattleGround를 모티브로 기획부터 시작해 캐릭터 이동, 총기, UI 등의 기능 구현, Component 디자인 패턴 적용을 시도해 개발했습니다. 팀 데스매치 모드로 게임을 즐길 수 있으며, 향후 모게임의 서바이벌 모드와 같은 기능을 추가할 예정입니다.


## 기간
2024.07. ~ 2024.12.


## 참여 인원 및 역할
총 4명
- 최정호(본인) : 팀장 - 캐릭터 및 총기 연동 기반
- 김연경 : 팀원 - 총기
- 곽경호 : 팀원 - 게임 내부 UI
- 김민재 : 팀원 - 로비 및 방 관련 UI


- 비중 : FE(35%), BE(35%), 기획(35%)

---


팀원 Git 링크
- 김연경 : https://github.com/yeonkyeong1022
- 김민재 : https://github.com/FineDust1598
- 곽경호 : https://github.com/gwag-gyeongho


## 언어
- C++ & Unreal C++

  
## 사용 툴
- Unreal Engine 5.2 Git


## 협업 툴
- Github : 협업 코드 저장소
- Notion : 기획 & 일정 관리

---
<table>
  <tr>
    <td style="text-align: center;">
      <figure>
        <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/ReadMe_Image_8.png?raw=true" alt="Image 8" width="500">
        <figcaption>Notion : 기획서 목차</figcaption>
      </figure>
    </td>
    <td>
      <figure>
        <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/ReadMe_Image_10.png?raw=true" alt="Image 10" width="500">
        <figcaption>Notion : 참고 애셋</figcaption>
      </figure>
    </td>
  </tr>
  <tr>
    <td style="text-align: center;">
      <figure>
        <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/ReadMe_Image_7.png?raw=true" alt="Image 7" width="500">
        <figcaption>Notion : 무기 기능 기획서 작성</figcaption>
      </figure>
    </td>
    <td>
      <figure>
        <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/ReadMe_Image_11.png?raw=true" alt="Image 11" width="500">
        <figcaption>Notion : 이동 기능 기획서 작성</figcaption>
      </figure>
    </td>
  </tr>
</table>


## 플로우 차트
![플로우 차트](https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/ReadMe_Image_1.png)


## 본인 역할
![본인 역할](https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaeng_DevelopmentPart.png)


캐릭터 이동, 파쿠르, 대쉬 스킬 등의 캐릭터 파트와 캐릭터 애니메이션 블루프린트에 총기를 사용할 수 있도록 Aim Offset을 추가하는 파트까지 담당했습니다.
현재 본인 파트는 개발을 완료했으며, 나머지 팀원들의 파트를 보조 및 피드백을 통해 총괄하고 있습니다.

### 기획
- 구현해야할 기능 목록을 정리하고 어떤 로직으로 구성되는지 예상하는 기획안 작성
- Unreal Engine의 무료 프로젝트인 'Lyra Project', 'Advanced Locomotion System V2'와 게임으로 출시된 'Play Unknown's BattleGround', 'Counter-Strike Online'의 일부분을 모티브, 레퍼런스로 활용해서 기획
- 기획안 페이지 : https://www.notion.so/7-8-c224a63038304dbb85ea3f5cf0149167?pvs=4


### Component 구조 적용
- Unreal Engine의 Static Mesh Component, Skeletal Mesh Component처럼 다른 클래스에 쉽게 연동할 수 있도록 결합도를 낮춘 Component 디자인 패턴을 적용 시도
- Delegate 사용해 Component간 결합도를 낮추기 위해 노력했습니다.


### 캐릭터 이동 & Dedicated Server 적용
<table>
  <tr>
    <td>
      <figure>
        <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaneg_Dedicated_Image.png?raw=true" alt="Image 12" width="500">
      </figure>
    </td>
    <td>
      <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaeng_GIF_Locomotion.gif?raw=true" alt="GIF Locomotion" width="500">
    </td>
  </tr>
</table>
- 캐릭터 이동과 관련된 기능을 구현
1. WASD키로 캐릭터 이동
2. Ctrl키로 캐릭터 앉기
3. 마우스로 캐릭터 시점 처리


### 시점 세부 처리
<table>
  <tr>
    <td>
      <figure>
        <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaeng_GIF_AimOffset_Idle.gif?raw=true" alt="GIF AimOffset Idle" width="500">
      </figure>
    </td>
    <td>
      <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaeng_GIF_AimOffset_Sit.gif?raw=true" alt="GIF AimOffset Sit" width="500">
    </td>
  </tr>
</table>
- 캐릭터의 이동이 없는 경우, 카메라 상하좌우 위치에 따라 캐릭터의 바라보는 방향을 세부적으로 처리


### 파쿠르
<table>
  <tr>
    <td>
      <figure>
        <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaneg_Parkour_Image.png?raw=true" alt="Image 13" width="500">
      </figure>
    </td>
    <td>
      <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaeng_GIF_Parkour.gif?raw=true" alt="GIF Parkour" width="500">
    </td>
  </tr>
</table>


![파쿠르 플로우 차트](https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/ReadMe_Image_2.png)
- 'Advanced Locomotion System V2'의 파쿠르를 모티브로 특정 높이 이하일 때, 캐릭터가 올라갈 수 있도록 구현


### 총기 연동
- 다양한 총기 기능에 대한 애니메이션 상호작용이 이뤄지도록 Animation Blueprint에서 State Machine에서 틀을 구현


### 대쉬 스킬
<table>
  <tr>
    <td>
      <figure>
        <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaneg_Dash_Image.png?raw=true" alt="Image 14" width="500">
      </figure>
    </td>
    <td>
      <img src = "https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaeng_GIF_DashSkill.gif?raw=true" alt="GIF DashSkill" width="500">
    </td>
  </tr>
</table>


<img src="https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/ReadMe_Image_3.png" width="800">
- 'Lyra Project'의 대쉬 이동을 모티브로 사용자의 WASD키와 Shift키를 누를 경우, 해당 방향으로 고속 이동하도록 구현


## 시연 영상
[![시연 영상](https://github.com/Freode/K_YangGaeng/blob/main/ReadMeImage/K_YangGaneg_Title_Image.png)](https://youtu.be/f4ixF1Mcdbc)
- 클릭하시면, 시연 영상으로 이동합니다.


## 느낀점


- 협업을 위해 Component 패턴을 사용해 구현 영역을 나누기를 시도하고 새로운 기능을 개발하면서, 한 층 더 게임 개발 실력을 향상시키는 계기가 되었습니다.
- 다음번에는 캐릭터 파트가 아니라 총기 파트를 담당해 개발하고 싶습니다.
- 개강 후, 팀장으로서 팀원들의 일정 관리을 통해 개발 속도를 조절하고 있습니다.
