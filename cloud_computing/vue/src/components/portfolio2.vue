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
          from: '2012',
          to: '2019'
        },
        items: [
          {
            nameMonth: '18.08 - 현재',
            title: '3학년 2학기 재학중',
            description:
              ''
          },
          {
            nameMonth: '16.07 - 18.04',
            title: '현역 입대 및 전역',
            description:
              ''
          },
          {
            nameMonth: '15.03',
            title: '한국산업기술대학교 편입',
            description:
              '한국산업기술대학교 컴퓨터공학과 3학년 학사편입'
          },
          {
            nameMonth: '12.03 - 14.02',
            title: '편입 준비 및 이전 학교 졸업',
            description:
              ''
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
