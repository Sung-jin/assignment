<template>
  <div>
    <Timeline
      :timeline-items="timelineItems"
      :message-when-no-items="messageWhenNoItems"/>
      <b-btn variant="primary" href="#/">Home</b-btn>
      <b-btn variant="primary" href="#/project">Project</b-btn>
  </div>
</template>

<script>
import Timeline from 'timeline-vuejs'

export default {
  name: 'portfolio',
  components: {
    Timeline
  },
  data: () => ({
    messageWhenNoItems: 'There arent items',
    timelineItems: [
      {
        year: {
          from: '2017',
          to: '2019'
        },
        items: [
          {
            nameMonth: '18.07 - 현재',
            title: '<a href="http://yapp.co.kr/">YAPP</a> 13기 백엔드 담당',
            description:
              '<br>대학생 연합 기업형 IT 동아리<br>안드로이드 백엔드 담당<br>Node.js, FireBase, Android 담당<br><a href="https://github.com/YAPP13th">Git</a>'
          },
          {
            nameMonth: '18.07.27 - 18.07.29',
            title: '<a href="https://www.facebook.com/unithonWithU/">Uniton</a> 7TH 특별상',
            description:
              '<br>대학생 연합 해커톤 안드로이드 기능 파트 개발<br>기획, 디자인, 개발 코딩 마라톤<br>주제 - 버킷리스트 계모임<br><a href="https://github.com/kuk941025/Unithon-Bucket101">Git</a>'
          },
          {
            nameMonth: '17.09 - 18.06',
            title: '졸업작품',
            description:
              '<br>라즈베리파이 3와 Android Things를 이용하여 스마트 미러 제작<br>서버와 통신하여 사용자에게 의상추천 및 각종 위젯 제공<br><a href="https://github.com/Sung-jin/assignment/tree/master/Senier_Project">Git</a>'
          },
          {
            nameMonth: '17.12 - 18.02',
            title: '위밋플레이스 - 현장실습',
            description:
              '<br>위치추천 웹 서비스 개발 담당<br>구글, T맵을 이용하여 사용자들의 위치를 기준으로 걸리는 시간 중간 위치를 추천해주는 서비스 제작<br>JavaScript, APM, AWS 사용'
          }
        ]
      }
    ]
  })
}
</script>

<style scoped>
.wrapper-timeline{
  width: 100%;
}
</style>
