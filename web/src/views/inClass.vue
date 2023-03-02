<template>
    <div class='class-table'>
        <div class='table-wrapper'>
            <div class='tabel-container'>
                <table>
                    <thead>
                        <tr>
                            <th>时间</th>
                            <th v-for='(week, index) in weeks'> {{ week }}</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr v-for='(classtime, classIndex) in time' :key='classIndex'>
                            <td>
                                <p>{{ '第' + (classIndex + 1) + '节' }}</p>
                                <p class='period'>{{ classtime }}</p>
                            </td>
                            <!-- 填写课程信息  -->
                            <td v-for='weekIndex in 7' :key='weekIndex' @click="clickClass($event)" ref="clear">
                                {{ filltable(classIndex * 7 + weekIndex) }}
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>
        <el-button-group>
            <el-button type="primary" icon="el-icon-arrow-left" @click="toLastweek()">上一周</el-button>
            <el-button type="primary" @click="toNextweek()">下一周<i
                    class="el-icon-arrow-right el-icon--right"></i></el-button>
        </el-button-group>
        <el-tag>第{{ curWeek }}周</el-tag>
        <!-- 弹窗 -->
        
        <el-dialog title="提示" :visible.sync="dialogVisible" width="30%" :before-close="handleClose">
            <el-descriptions title="垂直带边框列表" direction="vertical" :column="4" border>
                <el-descriptions-item label="用户名">kooriookami</el-descriptions-item>
                <el-descriptions-item label="手机号">18100000000</el-descriptions-item>
                <el-descriptions-item label="居住地" :span="2">苏州市</el-descriptions-item>
                <el-descriptions-item label="备注">
                    <el-tag size="small">学校</el-tag>
                </el-descriptions-item>
                <el-descriptions-item label="联系地址">江苏省苏州市吴中区吴中大道 1188 号</el-descriptions-item>
            </el-descriptions>
            <span slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible = false">取 消</el-button>
                <el-button type="primary" @click="dialogVisible = false">确 定</el-button>
            </span>
        </el-dialog>

    </div>
</template>
<script>
export default {
    data() {
        return {
            curWeek: 1,
            weeks: ['周一', '周二', '周三', '周四', '周五', '周六', '周日'],
            time: ['08:00-09:00', '09:00-10:00', '10:00-11:00', '11:00-12:00', '13:00-14:00', '14:00-15:00', '15:00-16:00', '16:00-17:00', '17:00-18:00'],
            classData: [{
                course_name: '计网',
                course_id: 1,
                week_schedule: [1, 2, 3, 4, 5],
                session: [1, 8, 15],
            }, {
                course_name: '计算机组成原理',
                course_id: 1,
                week_schedule: [1, 2, 3, 4],
                session: [3, 10], //注意这里周的排列是一行一行来的
            }],
            // curCell: 0,
            dialogVisible: false,
        };
    },
    methods: {
        toNextweek() {
            if (this.curWeek >= 19) {
                return;
            }
            this.curWeek++;
            this.curCell = 0;//重置单元格计数器
            this.$forceUpdate(); //重新刷新页面
        },
        toLastweek() {
            if (this.curWeek <= 1) {
                return;
            }
            this.curWeek--;
            this.curCell = 0;//重置单元格计数器
            this.$forceUpdate(); //重新刷新页面
        },
        clickClass(event) {
            //通过获得当前元素的文本，向后台发出get请求，获得该课程的信息，然后弹窗
            console.log(event.target.innerHTML);
            //点击弹窗
            this.dialogVisible = true; 
            console.log(this.curCell)
        },
        // 关闭弹窗
        handleClose(done) {
            done();
        },
    },
    computed: {
        filltable() {
            return function (cell) {
                // 因为现在数据中只有两种课程，所以这里i < 2
                for (let i = 0; i < 2; i++) {

                    //如果是周数组中有当前周
                    if (this.classData[i].week_schedule.indexOf(this.curWeek) != -1) {
                        //如果是节数组中有当前节
                        if (this.classData[i].session.indexOf(cell) != -1) {
                            return this.classData[i].course_name;
                        }
                    }
                }
                return '';
            }
            
        },
    }
};

</script>

<style lang='scss' scoped>
.class-table {
    .table-wrapper {
        width: 100%;
        height: 100%;
        overflow: auto;
    }

    .tabel-container {
        margin: 7px;

        table {
            table-layout: fixed;
            width: 100%;

            thead {
                background-color: #67a1ff;

                th {
                    color: #fff;
                    line-height: 17px;
                    font-weight: normal;
                }
            }

            tbody {
                background-color: #eaf2ff;

                td {
                    color: #677998;
                    line-height: 12px;
                }
            }

            th,
            td {
                width: 60px;
                padding: 12px 2px;
                font-size: 12px;
                text-align: center;
            }

            tr td:first-child {
                color: #333;

                .period {
                    font-size: 8px;
                }
            }
        }
    }
}
</style>