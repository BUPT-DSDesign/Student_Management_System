<template>
  <div>
    <h2 class="system-time">系统时间：第 {{week }} 周 — 星期 {{day }} — {{ hourmap() }}:{{ minutemap() }}:{{ secondmap() }}</h2>
    
    
    <el-slider v-model="speed" show-input input-size="mini" :max=100></el-slider>

    <el-input-number class="base-time-num" v-model="baseTimeRun" controls-position="right" :min="1" :max="60" size="small" placeholder="时间基数"></el-input-number>
    <span style="float: left;margin-top: 18px;margin-left:1px;color:white">(时间行走基数)</span>
  </div>
</template>
<script>
import { TimeStore } from "@/store/time"

const map = {
  0: '00',
  1: '01',
  2: '02',
  3: '03',
  4: '04',
  5: '05',
  6: '06',
  7: '07',
  8: '08',
  9: '09',
}

export default {
  data() {
    return {
      baseTimeRun: 1, // 时间行走基数
      speed: 1,
      week: 1,
      day: 1,
      hour: 0,
      minute: 0,
      second: 0
    }
  },
  mounted() {
    this.week = TimeStore.week
    this.day = TimeStore.day
    this.hour = TimeStore.hour
    this.minute = TimeStore.minute
    this.second = TimeStore.second

    setInterval(() => {
      // 根据speed和baseTimeRun计算时间
      TimeStore.runTime(this.speed, this.baseTimeRun)
      this.second = this.second + this.speed * this.baseTimeRun
      this.minute = this.minute + Math.floor(this.second / 60)
      this.second = this.second % 60
      this.hour = this.hour + Math.floor(this.minute / 60)
      this.minute = this.minute % 60
      this.day = this.day + Math.floor(this.hour / 24)
      this.hour = this.hour % 24
      this.week = this.week + Math.floor(this.day / 7)
      this.day = this.day % 7
  
    }, 1000)
  },
  methods: {

  },
  computed: {
    hourmap() {
      return function () {
        if (this.hour < 10) {
          return map[this.hour];
        }
        else
          return this.hour;
      }
    },
    minutemap() {
      return function () {
        if (this.minute < 10) {
          return map[this.minute];
        }
        else
          return this.minute;
      }
    },
    secondmap() {
      return function() {
        if (this.second < 10) {
          return map[this.second];
        }
        else {
          return this.second;
        }
      }
    }
  }
}

</script>
<style>
@font-face {
  font-family: mFont;
  src: url(@/assets/font/LcdD.ttf);
}

.system-time {
  margin-left: 20px;
  color: #eee;
  float: left;
  font-family: mFont;
  width: 350px;
}
.el-slider{
  margin-top:10px;
  width:200px;
  float: left;
}
.base-time-num {
  margin-left: 20px;
  float: left;
  margin-top: 13px;
  width : 80px;
}
</style>