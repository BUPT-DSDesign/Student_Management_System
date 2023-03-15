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
//计算当前时间距离开学是第几周的函数
function calculateWeek() {
  var start = '2023/2/20'
  var WEEKLEN = 7, // 一周7天为常量
    WEEKDAYS = ["日", "一", "二", "三", "四", "五", "六"],
    weekInfo = { "week": null, "day": null }, // 初始化返回信息，默认第null周，星期null
    oneDay = 24 * 60 * 60 * 1000, // 一天的毫秒时长
    weekLeave, // 开学当天所在周剩余天数
    weekStart, // 开学当天start是星期几
    today, // 今天
    dateDiff, // 今天与开学当天日期差
    sDate; //开学之日，日期对象
  var rDateStr = /\d{4}[\/-]\d{1,2}[\/-]\d{1,2}/g; // 简单的日期格式校验：2013/12/19
  if (!rDateStr.test(start)) {
    alert("请使用合法的开学日期！！！");
    return weekInfo;
  }
  sDate = new Date(start.replace("-", "/"));
  weekStart = sDate.getDay();
  weekStart = weekStart === 0 ? 7 : weekStart; // JS中周日的索引为0，这里转换为7，方便计算

  weekLeave = WEEKLEN - weekStart;
  today = new Date();
  weekInfo.day = WEEKDAYS[today.getDay()];
  today = new Date(today.getFullYear() + "/" + (today.getMonth() + 1) + "/" + today.getDate());
  dateDiff = today - sDate;
  dateDiff = parseInt(dateDiff / oneDay);
  weekInfo.week = Math.ceil((dateDiff - weekLeave) / WEEKLEN) + 1;
  return weekInfo;
}

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
        var date = new Date();
        var year = date.getFullYear();
        var month = date.getMonth();
        var day = date.getDate();
        var td = year + '/' + month + '/' + day;
        return calculateWeek(td);
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