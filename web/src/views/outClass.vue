<template>
    <div class="container">
        <div class="schedule-container">
            <h2>日程安排表</h2>
            <div>
                <el-table
                    :data="eventList.filter(data => !search || data.event.includes(search) || data.week.includes(search) || data.time.includes(search))"
                    style="width: 100%" max-height="350">
                    <el-table-column label="日期" prop="start_day" fixed sortable :formatter="timeFormatter"
                        :sort-method="(a, b) => a.week.localeCompare(b.week)">
                    </el-table-column>
                    <el-table-column label="时间" prop="start_time" fixed sortable
                        :sort-method="(a, b) => a.time.localeCompare(b.time)">
                    </el-table-column>
                    <el-table-column label="事件" prop="activity_name" fixed>
                    </el-table-column>
                    <el-table-column label="标签" prop="tag" width="100" :formatter="typeFormatter"
                        :filters="[{ text: '集体活动', value: 0 }, { text: '个人活动', value: 1 }]" :filter-method="filterTag"
                        filter-placement="bottom-end">
                    </el-table-column>
                    <el-table-column align="right">
                        <template slot="header" slot-scope="scope">
                            <el-input v-model="search" size="mini" placeholder="输入关键词(时间/事件)搜索" />
                        </template>
                        <template slot-scope="scope">
                            <el-button type="primary" size="mini" @click="handleClick(scope.row)">查看详情</el-button>
                            <!-- <el-button type="info" size="mini" @click="handleEdit(scope.$index, scope.row)">编辑</el-button> -->
                            <el-button type="danger" size="mini"
                                @click="handleDelete(scope.$index, scope.row)">删除</el-button>
                        </template>
                    </el-table-column>
                </el-table>
            </div>
        </div>
        <eventDialog :selected="selected" :dialogDetailVisible="dialogDetailVisible"></eventDialog>
        <!-- 按键部分 -->
        <div class="button-container">
            <!-- 添加活动按钮和弹窗 -->
            <el-button class="OperationButton" type="primary" icon="el-icon-circle-plus-outline"
                @click="dialogAddVisible = true">添加活动</el-button>
            <el-dialog title="添加活动" :visible.sync="dialogAddVisible" width="500px">
                <el-form :model="form" label-width="150px">
                    <el-form-item label="活动名称" >
                        <el-input v-model="form.event" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="活动地点" >
                        <el-input v-model="form.address" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="开始周次" >
                        <el-input v-model="form.start_week" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="结束周次" >
                        <el-input v-model="form.endweek" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="活动时间" >
                        <el-input v-model="form.start_time" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="活动类型"  >
                        <el-radio v-model="radio" label="1">个人活动</el-radio>
                        <el-radio v-model="radio" label="2">集体活动</el-radio>
                    </el-form-item>
                </el-form>
                <div slot="footer" class="dialog-footer">
                    <el-button @click="dialogAddVisible = false">取 消</el-button>
                    <el-button type="primary" @click="dialogAddVisible = false">添加</el-button>
                </div>
            </el-dialog>
            <!-- 闹钟提醒 -->
            <el-button class="OperationButton" type="primary" icon="el-icon-alarm-clock">闹钟提醒</el-button>
        </div>
    </div>
</template>

<script>
import eventDialog from '../components/eventDialog.vue'  //引入弹窗组件
import { EventStore } from '@/store/event';

export default {
    data() {
        return {
            selected: {},//选择查看详情的活动对象
            dialogDetailVisible: false,
            dialogAddVisible: false, //添加活动的弹窗是否可见
            //增加活动时表单中填的数据
            form: {
                start_week: '',
                start_day: '',
                start_time: '',
                event: '',
                tag: '',
                address: '',
                startweek: '',
                endweek: '',
            },
            radio: '',
            search: '', //用于搜索过滤的对象
            value1: [new Date(2016, 9, 10, 8, 40), new Date(2016, 9, 10, 9, 40)],
            eventList: [],
        }
    },
    created() {
        const getActivityTable = async () => {
            const fg = await EventStore.GetEventTable()
            if (fg) {
                this.eventList = EventStore.eventList
            } else {
                console.log('error')
            }
        }
        getActivityTable()
    },
    methods: {
        //规范化类型  
        typeFormatter(row) {
            switch (row.tag) {
                case 1:
                    return '个人活动';
                case 0:
                    return '集体活动'
            }
        },
        timeFormatter(row) {
            switch (row.start_day) {
                case 1:
                    return '周一';
                case 2:
                    return '周二';
                case 3:
                    return '周三';
                case 4:
                    return '周四';
                case 5:
                    return '周五';
                case 6:
                    return '周六';
                case 7:
                    return '周日';
            }
        },
        //删除活动按钮,row即为活动对象
        handleDelete(index, row) {
            console.log(row);
        },
        //对标签进行筛选
        filterTag(value, row) {
            return row.tag === value;
        },
        // 查看详情按钮
        handleClick(row) {
            this.selected = row;
            this.dialogDetailVisible = true;
        },

    },
    components: {
        eventDialog,
    }
}
</script>
<style>
.schedule-container {
    margin-top: 40px;
}

.el-input__inner {
    height: 39px;
    line-height: 30px;
}

h2 {
    margin: 15px;
    margin-left: 200px;
    font-size: 20px;
}

.button-container {
    margin-top: 20px;
    margin-left: 130px;
}

.OperationButton {
    margin: 20px
}

.weekinput {
    width: 30px;
    margin: 0 5px;
}
</style>