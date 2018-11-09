<template>
  <div class="total">
    <div class="outer">
      <div class="inner">
        <div class="center">
          <b-card-group deck>
            <b-card title="오성진"
            img-src="../assets/myProfile.jpg"
            img-alt="Image"
            img-top
            tag="article"
            style="max-width: 20rem;"
            class="mb-2">
            <p class="card-text">
              한국산업기술대학교 컴퓨터공학과<br>
              13학번 오성진
            </p>
            <b-button href="#/project" variant="primary">Project Info</b-button>
            <b-button href="#/portfolio" variant="primary">Portfolio</b-button>
            </b-card>

            <b-card title="나영훈"
            img-src="../assets/myProfile3.jpg"
            img-alt="Image"
            img-top
            tag="article"
            style="max-width: 20rem;"
            class="mb-2">
            <p class="card-text">
              한국산업기술대학교 컴퓨터공학과<br>
              13학번 나영훈
            </p>
            <b-button href="#/project" variant="primary">Project Info</b-button>
            <b-button href="#/portfolio2" variant="primary">Portfolio</b-button>
            </b-card>
          </b-card-group>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import bButton from 'bootstrap-vue/es/components/button/button'

export default {
  name: 'Main',
  components: {
    bButton
  },
  data () {
    return {
      size: 'lg'
    }
  }
}
</script>

<style scoped>
.total {
  width: 100%;
  height: 100%;
  background: #1bbc9b;
}
.outer{
  display: table;
  width: 100%;
  height: 100%;
}
.inner{
  display: table-cell;
  vertical-align: middle;
  text-align: center;
}
.center{
  position: relative;
  display: inline-block;
}
</style>
