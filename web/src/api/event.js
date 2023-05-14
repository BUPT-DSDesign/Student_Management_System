import { service } from '@/utils/request'

// 课外活动接口
export const EventTable = () => {
    return service({
        url: '/activity/info',
        method: 'get'
    })
}
// 添加活动
export const AddEvent = (data) => {
    return service({
        url: '/event/add',
        method: 'post',
        data: data
    })
}

// 删除课程
export const DeleteCourse = (data) => {
    return service({
        url: '/course/delete',
        method: 'post',
        data: data
    })
}