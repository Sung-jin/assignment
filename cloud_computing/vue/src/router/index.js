import Vue from 'vue'
import Router from 'vue-router'
import Main from '@/components/Main'
import Project from '@/components/projectInfo'
import Portfolio from '@/components/portfolio'
import Portfolio2 from '@/components/portfolio2'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'Main',
      component: Main
    },
    {
      path: '/project',
      name: 'Project',
      component: Project
    },
    {
      path: '/portfolio',
      name: 'Portfolio',
      component: Portfolio
    },
    {
      path: '/portfolio2',
      name: 'Portfolio2',
      component: Portfolio2
    }
  ]
})
