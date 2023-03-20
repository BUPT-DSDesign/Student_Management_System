import { service } from '@/utils/request'

// 课外活动接口
export const EventTable = () => {
    return service({
        url: '/activity',
        method: 'get'
    })
}