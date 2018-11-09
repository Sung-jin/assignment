<template>
  <div class="total">
    <b-jumbotron class="b-jumbotron" bg-variant="secondary" text-variant="white">
      <template slot="header">
        Project Info
      </template>
      <template slot="lead">
        웹페이지 화면 3개 이상으로 구성<br>
        팀원 사진 2개이상 포함<br>
        AWS 서비스 - EC2, Elastic IP, 보안그룹, 키페어 사용
      </template>
      <hr class="my-4">
      <p>
        Node.js 8.12.0 LTS
      </p>
      <p>
        Vue.js 2.9.6
      </p>
      <p>
        bootstrap-vue
      </p>
      <p>
        EC2 - ubuntu 16.04 LTS
      </p>
      <b-btn variant="primary" href="#/">Home</b-btn>
      <b-btn variant="primary" href="#/portfolio">SungJin's Portfolio</b-btn>
      <b-btn variant="primary" href="#/portfolio2">YeongHun's Portfolio</b-btn>
    </b-jumbotron>
  </div>
</template>

<script>

export default {
  name: 'projectInfo'
}
</script>

<style scoped>
.total {
  width: 100%;
  height: 100%;
}
.b-jumbotron{
  margin: 0px;
  width: 100%;
  height: 100%;
}
</style>
