<template>
  <div class="header">
    <div class="left">
      <h1 class="weektitle">第{{ curweek().week }}周 星期{{ curweek().day }}</h1>
    </div>
    <div class="right">
      <el-dropdown>
        <span class="el-dropdown-link">
          <img class="icon" :src="avatarUrl" @error="changeToDefault">
        </span>
        <el-dropdown-menu>
          <el-dropdown-item>个人中心</el-dropdown-item>
          <el-dropdown-item @click.native="layout">退出</el-dropdown-item>
        </el-dropdown-menu>
      </el-dropdown>
    </div>
  </div>
</template>
<script>
import { calcurWeek } from "@/utils/time"
export default {
  data() {
    return {
      avatarUrl: '',// 默认头像
    }
  },
  mounted() {
    const userId = window.localStorage.getItem('userId')
    this.avatarUrl = `http://127.0.0.1:8080/static/${userId}.jpg`
  },
  computed: {
    curweek() {
      return function () {
        return calcurWeek();
      }
    }
  },
  methods: {
    changeToDefault() {
      this.avatarUrl = `http://127.0.0.1:8080/static/avatar.jpg`
    },
    layout() {
      window.localStorage.removeItem('token') //移除token
      window.localStorage.removeItem('userId') //移除id
      this.$router.replace('/')
    },
  },
}
</script>
<style>
.header {
  background-color: #343a40;
  height: 54px;
}

.el-icon-arrow-down {
  font-size: 12px;
}

.right {
  float: right;
  margin: 10px;
}
.weektitle{
  margin-left:20px;
  margin-top:10px;
  color: #eee;
  float: left;
}
.icon {
  width: 35px;
  height: 35px;
  margin-right: 20px;
  border-radius: 50%;
}
</style>